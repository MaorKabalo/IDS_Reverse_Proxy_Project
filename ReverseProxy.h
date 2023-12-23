// ReverseProxy.h

#pragma once

#include <boost/asio.hpp>
#include <memory>



class ReverseProxyConnection : public std::enable_shared_from_this<ReverseProxyConnection> {
public:
    ReverseProxyConnection(boost::asio::io_context& io_context, const std::string& server_ip, short server_port, boost::asio::ip::tcp::socket* sock);
    void Start();

private:
    void StartRead();
    void ForwardToServer(const std::string& message);
    void close();

    boost::asio::ip::tcp::socket* client_socket_;
    boost::asio::ip::tcp::socket* server_socket_;
    boost::asio::streambuf input_buffer_;
    std::string server_ip_;
    short server_port_;
};

class ReverseProxy {
public:
    ReverseProxy(boost::asio::io_context& io_context, short proxy_port, const std::string& server_ip, short server_port);

private:
    void StartAccept();

    boost::asio::ip::tcp::acceptor acceptor_;
    std::string server_ip_;
    short server_port_;
};
