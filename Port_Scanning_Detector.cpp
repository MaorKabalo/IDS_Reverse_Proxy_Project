//
// Created by kali on 2/12/24.
//

#include "Port_Scanning_Detector.h"

#include "BlockIP.h"

#define PORT 100
#define PER_MS 15

int Port_Scanning_Detector::portsScannedCount = 0;
std::unordered_set<uint16_t> Port_Scanning_Detector::m_Ports;
std::chrono::steady_clock::time_point Port_Scanning_Detector::lastPacketTime;
std::unordered_set<std::string> Port_Scanning_Detector::mMalicousIPs;

std::mutex countPortMutex;
std::mutex mMalicousIPsMutex;
std::mutex lastPacketTimeMutex;


int countPort = 0;
//After learning the attack, it goes by a random ports in nmapDefault.txt




void Port_Scanning_Detector::extractPorts() {
    std::ifstream file(NMAP_PORTS_TXT);
    uint16_t port;
    while (file >> port) {
        m_Ports.insert(port);
    }
}




void Port_Scanning_Detector::onPacketArrives(RawPacket* packet, PcapLiveDevice* dev, void* cookie) {
    // Parse and process the packet
    Packet parsedPacket(packet);

    ProtocolType transportProtocol = parsedPacket.getLayerOfType<TcpLayer>()->getProtocol();
    auto* tcpLayer = parsedPacket.getLayerOfType<TcpLayer>();
    auto* ipLayer = parsedPacket.getLayerOfType<IPv4Layer>();

    uint32_t sourceIP = ipLayer->getIPv4Header()->ipSrc;

    if (transportProtocol == TCP) {
        uint16_t destPort = ntohs(tcpLayer->getTcpHeader()->portDst);

        // Safely increment countPort using a mutex
        {
            std::lock_guard<std::mutex> lock(countPortMutex);
            if (m_Ports.find(destPort) != m_Ports.end()) {
                countPort++;
            }
        }

    }

    auto currentTime = std::chrono::steady_clock::now();
    auto timeDifference = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastPacketTime).count();

    // Safely check and update malicious IPs using a mutex
    if (timeDifference > PER_MS) {
        {
            std::lock_guard<std::mutex> lock(countPortMutex);
            if (countPort >= PORT) {
                std::cout << "Port Scanning Detected!" << std::endl;

                BlockIP::blockIP(IPTools::intToIPv4String(sourceIP).c_str());


                // mMalicousIPs.insert(IPTools::intToIPv4String(sourceIP));
                //
                // // Safely print malicious IPs using a mutex
                // std::lock_guard<std::mutex> ipLock(mMalicousIPsMutex);
                // for (const auto& ip : mMalicousIPs) {
                //     std::cout << "IP: " << ip << std::endl;
                // }
            }
        }

        // Safely reset counters and update lastPacketTime using mutexes
        {
            std::lock_guard<std::mutex> lock(countPortMutex);
            countPort = 0;
        }

        {
            std::lock_guard<std::mutex> lock(lastPacketTimeMutex);
            lastPacketTime = currentTime;
        }
    }
}




Port_Scanning_Detector::Port_Scanning_Detector(std::string interfaceName) {

    std::thread thread(&Port_Scanning_Detector::extractPorts);
    thread.detach();

    m_InterfaceName = std::move(interfaceName);

    PcapLiveDeviceList& devList = PcapLiveDeviceList::getInstance();
    int interfaceIndex = -1;

    int i = 0;
    for (const PcapLiveDevice* dev : devList.getPcapLiveDevicesList()) {
        if (dev->getName() == m_InterfaceName) {
            interfaceIndex = i;
        }
        i++;
    }



    try {
        m_PcapLiveDevice = devList.getPcapLiveDevicesList()[interfaceIndex];

        if (m_PcapLiveDevice == nullptr || interfaceIndex == -1) {
            throw std::runtime_error("Invalid PcapLiveDevice");
        }
        m_PcapLiveDevice->open();
    } catch (const std::exception& exc) {
        std::cout << "Error: " << exc.what() << std::endl;
        std::cout << "Choose one of the available network interfaces:" << std::endl;
        for (const PcapLiveDevice* dev : devList.getPcapLiveDevicesList()) {
            std::cout << dev->getName() << std::endl;
        }
        return;
    }

    //std::cout << "Port Scanning Prevention Initialized!" << std::endl;
}

Port_Scanning_Detector::~Port_Scanning_Detector() {
    if (m_PcapLiveDevice != nullptr) {
        m_PcapLiveDevice->close();
    }
}

void Port_Scanning_Detector::ListenForSYNScanAttack() const {

    std::cout << "Port Scanner Listening on interface: " << m_PcapLiveDevice->getName() << std::endl;


    m_PcapLiveDevice->startCapture(onPacketArrives, nullptr);
    //std::this_thread::sleep_for(std::chrono::seconds(100));



    // Start capturing with the callback function

    //std::cout << "Press Enter to stop capturing..." << std::endl;
    std::cin.get();


    // while (!stopCapture) {
    //     std::this_thread::sleep_for(std::chrono::milliseconds(100));
    // }

    // Wait for user input or some other event to stop capturing


    // Stop capturing and close the interface

}

