// ReverseProxy.cpp

#include "ReverseProxy.h"
#include <iostream>

#include "SockLimiting.h"

ReverseProxyConnection::ReverseProxyConnection(boost::asio::io_context& io_context, const std::string& server_ip, short server_port, boost::asio::ip::tcp::socket* sock)
    : client_socket_(sock),
      server_socket_(sock),
      server_ip_(server_ip),
server_port_(server_port) {}

void ReverseProxyConnection::Start() {
    std::cout << "New connection" << std::endl;
    StartRead();
}

void ReverseProxyConnection::StartRead() {
    try {
        auto self = shared_from_this();
        constexpr std::size_t maxBytesToRead = 1024;  // Adjust as needed
        boost::asio::async_read(
            (*client_socket_), input_buffer_, boost::asio::transfer_at_least(1),
            [self, maxBytesToRead](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    std::istream is(&self->input_buffer_);
                    std::string message;
                    message.resize(std::min(length, maxBytesToRead));
                    is.read(&message[0], static_cast<std::streamsize>(length));

                    std::cout << "Received: " << message;



                    self->ForwardToServer(message);

                    // Check if the client socket is still open before starting another read
                    //if (self->client_socket_->is_open()) {
                        self->StartRead();
                    //}
                } else if (ec != boost::asio::error::operation_aborted) {
                    std::cout << "Error in StartRead: " << ec.message() << std::endl;
                }
            }
        );
    } catch (const std::exception& e) {
        std::cout << "Exception in StartRead: " << e.what() << std::endl;
    }
}





void ReverseProxyConnection::ForwardToServer(const std::string& message) {
    try {
        boost::asio::ip::tcp::resolver resolver(client_socket_->get_executor());
        auto endpoints = resolver.resolve(server_ip_, std::to_string(server_port_));
        auto self = shared_from_this();
        boost::asio::async_connect((*server_socket_), endpoints,
            [self, message](boost::system::error_code ec, boost::asio::ip::tcp::endpoint) {
                if (!ec) {
                    boost::asio::async_write(*(self->server_socket_), boost::asio::buffer(message),
                        [self](boost::system::error_code ec, std::size_t /*length*/) {
                            if (!ec) {
                                std::cout << "Forwarded message to server" << std::endl;
                                //self->StartRead();
                            } else {
                                std::cout << "Failed to forward message to server: " << ec.message() << std::endl;
                            }
                        });
                } else {
                    std::cout << "Failed to connect to server: " << ec.message() << std::endl;
                }
            });
    } catch (const std::exception& e) {
        std::cout << "Exception while forwarding to server: " << e.what() << std::endl;
    }
}

void ReverseProxyConnection::close()
{
    //this->client_socket_->close();
    //this->server_socket_->close();
}

ReverseProxy::ReverseProxy(boost::asio::io_context& io_context, short proxy_port, const std::string& server_ip, short server_port)
    : acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), proxy_port)),
      server_ip_(server_ip),
      server_port_(server_port) {
    StartAccept();
}

void ReverseProxy::StartAccept() {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
            if (!ec&&SockLimiting::add(socket.remote_endpoint().address().to_string())) {

                //boost::asio::ip::tcp::socket* sock = (boost::asio::ip::tcp::socket* )malloc(sizeof(socket));
                //sock->assign(boost::asio::ip::tcp::v4(), socket.native_handle());

                boost::asio::io_context& io_context1 = static_cast<boost::asio::io_context&>(socket.get_executor().context());
                auto connection = std::make_shared<ReverseProxyConnection>(io_context1, server_ip_, server_port_, &socket);
                connection->Start();
                //io_context1.run_one();
                io_context1.run_for(std::chrono::seconds(6));
                //io_context1.run();

            }
            {
                //std::cout <<"error: "<< ec.message() << std::endl;
            }
            StartAccept();
        });
}

