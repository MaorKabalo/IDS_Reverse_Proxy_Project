#include "ReverseProxy.h"

#include <algorithm>
#include <cstring>
#include <chrono>

std::mutex ReverseProxy::m_mutex;
int ReverseProxy::m_numOfClient = 0;
//const Port_Scanning_Detector ReverseProxy::detector(INTERFACE_FOR_PORT_SCANNING);

ReverseProxy::ReverseProxy() : m_detector(INTERFACE_FOR_PORT_SCANNING)
{
    m_proxyClientSocket = socket(AF_INET, SOCK_STREAM, 0); // Use 0 for protocol since TCP is the default

    if (m_proxyClientSocket == -1)
        throw std::runtime_error(std::string(__func__) + " - socket");


    std::thread thread(&Port_Scanning_Detector::ListenForSYNScanAttack, &m_detector);
    thread.detach();

    //std::thread([this] { m_detector.ListenForSYNScanAttack(); }).detach();


}

ReverseProxy::~ReverseProxy()
{
    try
    {
        // Freeing resources allocated in the constructor
        close(m_proxyServerSocket);
        for (auto& pair : m_clients) {
            close(pair.first);
        }
        close(m_proxyClientSocket);
    }
    catch (...) {}
}

void ReverseProxy::bindAndListen() const
{
    struct sockaddr_in sa = { 0 };

    sa.sin_port = htons(PROXY_PORT);
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = INADDR_ANY;

    if (bind(m_proxyClientSocket, (struct sockaddr*)&sa, sizeof(sa)) == -1)
        throw std::runtime_error(std::string(__func__) + " - bind");

    if (listen(m_proxyClientSocket, SOMAXCONN) == -1)
        throw std::runtime_error(std::string(__func__) +" - listen");


    std::cout << "Listening on port " << PROXY_PORT << std::endl;

}

void ReverseProxy::initProxyServerSocket() {

    // Create a socket to connect to the server
    m_proxyServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_proxyServerSocket == -1)
    {
        throw std::runtime_error("Failed to create server socket");
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, ADDRESS, &serverAddr.sin_addr) <= 0)
    {
        throw std::runtime_error("Invalid server address");
    }

    // Connect to the server
    if (connect(m_proxyServerSocket, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1)
    {
        throw std::runtime_error("Failed to connect to the server");
    }
    std::cout << "Established connection to server!" << std::endl;
}


void ReverseProxy::startHandleRequests() {
    this->bindAndListen();
    this->initProxyServerSocket();

    while (true) {
        std::cout << "Waiting for client connection request" << std::endl;

        int client_socket = accept(m_proxyClientSocket, nullptr, nullptr);
        if (client_socket == -1)
            throw std::runtime_error(std::string(__func__));
        char ipAdress[11];
        ReverseProxy::get_socket_ip(client_socket, ipAdress);
        if(SockLimiting::add(std::string(ipAdress)))
        {
            throw std::runtime_error("max soket");
        }

        std::cout << "Client accepted. Server and client can speak" << std::endl;

        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_clients.insert({ client_socket, m_numOfClient++ });
        }

        std::thread client_thread(&ReverseProxy::handleNewClient, this, client_socket);
        client_thread.detach();
    }
}

bool isNumeric(const std::string& str)
{
    return std::all_of(str.begin(), str.end(), [](char c) {
        return std::isdigit(c);
    });
}


void ReverseProxy::handleNewClient(const int clientSocket) {
    try {
        while (true) {
            std::string m = receiveStringFromSocket(clientSocket, this->m_clients);  //getting request
            if (m.empty()) { throw std::runtime_error("Client exits the program"); }
            std::cout << m << std::endl;
            forwardToServer(m);
        }

        {
            std::lock_guard<std::mutex> lock(m_mutex);
            // Closing the socket (in the level of the TCP protocol)
            m_clients.erase(clientSocket);
        }
        close(clientSocket);
    }
    catch (const std::exception& e) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_clients.erase(clientSocket);
        close(clientSocket);
        std::cerr << "Error handling client: " << e.what() << std::endl;
    }
    catch (...) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_clients.erase(clientSocket);
        close(clientSocket);
        std::cerr << "Unknown error handling client." << std::endl;
    }
}

std::string ReverseProxy::receiveStringFromSocket(const int socket, std::map<int,int>& c)
{
    std::string result;
    char buffer[1024];
    int bytes_received;
    std::chrono::steady_clock::time_point last_packet_time;

    do {
        bytes_received = recv(socket, buffer, sizeof(buffer), 0);
        if (bytes_received > 0) {
            result += std::string(buffer, bytes_received);
            last_packet_time = std::chrono::steady_clock::now();
        }
        else if (bytes_received == 0) {
            auto now = std::chrono::steady_clock::now();
            auto time_diff = std::chrono::duration_cast<std::chrono::microseconds>(now - last_packet_time).count();
            if(time_diff>=MAX_TIME_MS)
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                c.erase(socket);
                close(socket);
                throw std::runtime_error("time out");
            }
            break;
        }
        else if (bytes_received == -1) {
            if (errno == EWOULDBLOCK) {
                continue;
            }
            else {
                std::cerr << "Error receiving data: " << strerror(errno) << std::endl;
            }
            break;
        }
    } while (bytes_received == sizeof(buffer));

    return result;
}

void ReverseProxy::forwardToServer(const std::string& message) const
{
    // Send the message to the server
    if (send(m_proxyServerSocket, message.c_str(), message.size(), 0) == -1)
    {
        throw std::runtime_error("Failed to send message to the server");
    }
}

int ReverseProxy::get_socket_ip(int sockfd, char* ip) {
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    if (getsockname(sockfd, (struct sockaddr*)&addr, &len) < 0) {
        perror("getsockname");
        return -1;
    }
    if (inet_ntop(AF_INET, &addr.sin_addr, ip, INET_ADDRSTRLEN) == NULL) {
        perror("inet_ntop");
        return -1;
    }
    return 0;
}
