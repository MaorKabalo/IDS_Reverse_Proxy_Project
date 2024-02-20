//
// Created by kali on 2/12/24.
//

#include "Port_Scanning_Detector.h"

int Port_Scanning_Detector::portsScannedCount = 0;
std::unordered_set<uint16_t> Port_Scanning_Detector::m_Ports;
std::chrono::steady_clock::time_point Port_Scanning_Detector::lastPacketTime;
std::mutex Port_Scanning_Detector::mutex;

//After learning the attack, it goes by a random ports in nmapDefault.txt

void Port_Scanning_Detector::extractPorts() {
    std::ifstream file(NMAP_PORTS_TXT);
    uint16_t port;
    while (file >> port) {
        m_Ports.insert(port);
    }
}

void Port_Scanning_Detector::onPacketArrives(RawPacket* packet, PcapLiveDevice* dev, void* cookie) {
    std::lock_guard<std::mutex> lock(mutex);  // Ensure thread safety

    // Parse and process the packet
    Packet parsedPacket(packet);

    ProtocolType transportProtocol = parsedPacket.getLayerOfType<TcpLayer>()->getProtocol();
    auto* tcpLayer = parsedPacket.getLayerOfType<TcpLayer>();

    if (transportProtocol == TCP) {
        uint16_t destPort = ntohs(tcpLayer->getTcpHeader()->portDst);

        if (m_Ports.find(destPort) != m_Ports.end()) {
            std::cout << destPort << std::endl;

            // Update the packet count and timestamp for port scanning detection
            // portsScannedCount++;
            // lastPacketTime = std::chrono::steady_clock::now();
            //
            // // Check for port scanning detection
            // if (isPortScanningDetected()) {
            //     std::cout << "Port Scanning detected" << std::endl;
            //     // Take appropriate action when port scanning is detected
            // }
        }
    }
}

// bool Port_Scanning_Detector::isPortScanningDetected() {
//     // Check if more than 100 packets have been received within half a second
//     auto currentTime = std::chrono::steady_clock::now();
//     auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastPacketTime).count();
//
//     if (elapsedTime > 500) {
//         // Reset packetCount if more than half a second has passed
//         portsScannedCount = 0;
//         lastPacketTime = currentTime;
//     }
//
//     return (portsScannedCount > 100);
// }




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

    std::cout << "Port Scanning Prevention Initialized!" << std::endl;
}

Port_Scanning_Detector::~Port_Scanning_Detector() {
    if (m_PcapLiveDevice != nullptr) {
        m_PcapLiveDevice->close();
    }
}

void Port_Scanning_Detector::ListenForSYNScanAttack() const {

    std::cout << "Listening on interface: " << m_PcapLiveDevice->getName() << std::endl;


    m_PcapLiveDevice->startCapture(onPacketArrives, nullptr);
    std::this_thread::sleep_for(std::chrono::seconds(100));



    // Start capturing with the callback function

    std::cout << "Press Enter to stop capturing..." << std::endl;
    std::cin.get();


    // while (!stopCapture) {
    //     std::this_thread::sleep_for(std::chrono::milliseconds(100));
    // }

    // Wait for user input or some other event to stop capturing


    // Stop capturing and close the interface

}

