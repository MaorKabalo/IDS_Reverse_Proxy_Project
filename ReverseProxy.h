#pragma once

#include <iostream>
#include <string>
#include <thread>
#include <deque>
#include <map>
#include <stdexcept>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <mutex>

#define ADDRESS "127.0.0.1"
#define PROXY_PORT 9090
#define SERVER_PORT 8888

class ReverseProxy
{
public:
    ReverseProxy();
    ~ReverseProxy();

    void startHandleRequests();

private:
    void bindAndListen() const;
    void handleNewClient(int clientSocket);
    void initProxyServerSocket();
    void forwardToServer(const std::string& message) const;
    static std::string receiveStringFromSocket(int socket);

    int m_proxyClientSocket;
    int m_proxyServerSocket{};

    std::map<int, int> m_clients;
    static int m_numOfClient;

    static std::mutex m_mutex;
};
