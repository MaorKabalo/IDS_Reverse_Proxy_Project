#ifndef REVERSE_PROXY_H
#define REVERSE_PROXY_H

#include <string>
#include <boost/asio.hpp>

class ReverseProxy {
public:
    ReverseProxy(boost::asio::io_context& io_context, short proxy_port, const std::string& server_ip, short server_port);

private:
    void StartAccept();
    void StartRead();
    void ForwardToServer(const std::string& message);
    //void connect();
    //void handle_client(std::shared_ptr<boost::asio::ip::tcp::socket> socket);

    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf input_buffer_;
    std::string server_ip_;
    short server_port_;
};

#endif // REVERSE_PROXY_H
