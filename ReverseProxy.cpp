#include "ReverseProxy.h"
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

ReverseProxy::ReverseProxy(boost::asio::io_context& io_context, short proxy_port, const std::string& server_ip, short server_port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), proxy_port)),
    socket_(io_context),
    server_ip_(server_ip),
    server_port_(server_port) {
    StartAccept();
}

void ReverseProxy::StartAccept() {
    acceptor_.async_accept(socket_,
        [this](boost::system::error_code ec) {
            if (!ec) {
                std::cout << "Received a connection" << std::endl;
                StartRead();
            }
            StartAccept();
        });
}

void ReverseProxy::StartRead() {
    boost::asio::async_read_until(socket_, input_buffer_, '\n',
        [this](boost::system::error_code ec, std::size_t length) {
            if (!ec) {
                std::istream is(&input_buffer_);
                std::string message;
                std::getline(is, message);
                std::cout << "Received: " << message << std::endl;

                ForwardToServer(message);

                StartRead();
            }
        });
}

void ReverseProxy::ForwardToServer(const std::string& message) {
    boost::asio::io_context io_context;
    tcp::socket server_socket(io_context);
    tcp::resolver resolver(io_context);

    try {
        tcp::resolver::results_type endpoints = resolver.resolve(server_ip_, std::to_string(server_port_));
        boost::asio::connect(server_socket, endpoints);

        boost::asio::write(server_socket, boost::asio::buffer(message));
        std::cout << "Forwarded message to server" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Server is not available: " << e.what() << std::endl;
    }
}

/*void connect()
{
    const unsigned short port = 9090;
    // Create a Boost.Asio io_context object
    boost::asio::io_context io_context;

    // Create an acceptor that listens on the specified port
    boost::asio::ip::tcp::acceptor acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));

    std::cout << "Server listening on port " << port << std::endl;

    while (true) {
        // Create a socket for an incoming connection
        std::shared_ptr<boost::asio::ip::tcp::socket> socket = std::make_shared<boost::asio::ip::tcp::socket>(io_context);

        // Wait for and accept an incoming connection
        boost::asio::ip::tcp::acceptor.accept(*socket);

        // Spawn a new thread to handle the connection
        std::thread(handle_client, socket).detach();
    }
}

void handle_client(std::shared_ptr<boost::asio::ip::tcp::socket> socket) {
    try {
        // Get the remote endpoint information (IP address and port)
        boost::asio::ip::tcp::endpoint remote_endpoint = socket->remote_endpoint();
        std::cout << "Connected to: " << remote_endpoint.address().to_string() << ":" << remote_endpoint.port() << std::endl;

        // Read data from the client and save it into a string
        std::string data;
        char buffer[1024];
        size_t length;
        do {
            length = socket->read_some(buffer, boost::system::error_code());
            data.append(buffer, length);
        } while (length > 0);
        // Print and save the received data
        std::cout << "Received: " << data << std::endl;

        //if(data.)

        // Close the socket
        socket->close();
    } catch (std::exception& e) {
        std::cerr << "Exception in thread: " << e.what() << std::endl;
    }
}*/