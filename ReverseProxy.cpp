#include "ReverseProxy.h"

#include <algorithm>
#include <cstring>

int ReverseProxy::m_numOfClient = 0;

ReverseProxy::ReverseProxy()
{
    m_serverSocket = socket(AF_INET, SOCK_STREAM, 0); // Use 0 for protocol since TCP is the default

    if (m_serverSocket == -1)
        throw std::runtime_error(std::string(__func__) + " - socket");
}

ReverseProxy::~ReverseProxy()
{
    try
    {
        // Freeing resources allocated in the constructor
        close(m_serverSocket);
    }
    catch (...) {}
}

void ReverseProxy::bindAndListen() const
{
    struct sockaddr_in sa = { 0 };

    sa.sin_port = htons(PROXY_PORT);
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = INADDR_ANY;

    if (bind(m_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == -1)
        throw std::runtime_error(std::string(__func__) + " - bind");

    if (listen(m_serverSocket, SOMAXCONN) == -1)
        throw std::runtime_error(std::string(__func__) +" - listen");

    std::cout << "Listening on port " << PROXY_PORT << std::endl;
}

void ReverseProxy::startHandleRequests()
{
    this->bindAndListen();
    std::cout << "Waiting for client connection request" << std::endl;

    while (true)
    {
        std::cout << "Waiting for client connection request" << std::endl;

        int client_socket = accept(m_serverSocket, nullptr, nullptr);
        if (client_socket == -1)
            throw std::runtime_error(std::string(__func__));

        std::cout << "Client accepted. Server and client can speak" << std::endl;

        m_clients.insert({ client_socket, m_numOfClient++ });

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

void ReverseProxy::handleNewClient(const int clientSocket)
{
    try
    {
        while (true)
        {
            std::string m = receiveStringFromSocket(clientSocket);  //getting request
            if (m.empty()) { throw std::runtime_error("Client exits the program"); }
            std::cout << m << std::endl;
            forwardToServer(clientSocket, m);
        }

        // Closing the socket (in the level of the TCP protocol)
        m_clients.erase(clientSocket);
        close(clientSocket);
    }
    catch (const std::exception& e)
    {
        m_clients.erase(clientSocket);
        close(clientSocket);
        std::cerr << "Error handling client: " << e.what() << std::endl;
    }
    catch (...)
    {
        m_clients.erase(clientSocket);
        close(clientSocket);
        std::cerr << "Unknown error handling client." << std::endl;
    }
}

std::string ReverseProxy::receiveStringFromSocket(const int socket)
{
    std::string result;
    char buffer[1024];
    int bytes_received;

    do {
        bytes_received = recv(socket, buffer, sizeof(buffer), 0);
        if (bytes_received > 0) {
            result += std::string(buffer, bytes_received);
        }
        else if (bytes_received == 0) {
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

void ReverseProxy::forwardToServer(int clientSocket, const std::string& message)
{
    // You may need to replace "SERVER_ADDRESS" and "SERVER_PORT" with your actual server's address and port

    // Create a socket to connect to the server
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
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
    if (connect(serverSocket, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1)
    {
        throw std::runtime_error("Failed to connect to the server");
    }

    // Send the message to the server
    if (send(serverSocket, message.c_str(), message.size(), 0) == -1)
    {
        throw std::runtime_error("Failed to send message to the server");
    }

    // Close the connection to the server
    close(serverSocket);
}
