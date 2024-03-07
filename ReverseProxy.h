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
#include "SockLimiting.h"

#include "Port_Scanning_Detector.h"

#define ADDRESS "127.0.0.1"
#define PROXY_PORT 9090
#define SERVER_PORT 8888
#define INTERFACE_FOR_PORT_SCANNING "lo"
#define MAX_TIME_MS 1800000//half an hour

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
    static std::string receiveStringFromSocket(int socket, std::map<int,int>& c);
    static int get_socket_ip(int sockfd, char* ip);

    int m_proxyClientSocket;
    int m_proxyServerSocket{};

    std::map<int, int> m_clients;
    static int m_numOfClient;
    static std::mutex m_mutex;
    const Port_Scanning_Detector m_detector;

};
