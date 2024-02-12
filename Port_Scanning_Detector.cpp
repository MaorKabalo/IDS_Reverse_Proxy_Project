//
// Created by kali on 2/12/24.
//

// #include "Port_Scanning_Detector.h"
// #include <iostream>
// #include <utility>
//
// Port_Scanning_Detector::Port_Scanning_Detector(std::string  interfaceName) : m_InterfaceName(std::move(interfaceName)), m_PcapLiveDevice(nullptr) {
//     // Open the network interface for capturing
//     m_PcapLiveDevice = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByName(m_InterfaceName);
//     if (m_PcapLiveDevice == nullptr || !m_PcapLiveDevice->open()) {
//         std::cerr << "Error opening network interface: " << m_InterfaceName << std::endl;
//         // Handle the error as needed
//     }
// }
//
// Port_Scanning_Detector::~Port_Scanning_Detector() {
//     // Close the network interface when the object is destroyed
//     if (m_PcapLiveDevice != nullptr) {
//         m_PcapLiveDevice->close();
//     }
// }
//
// void Port_Scanning_Detector::packetHandler(pcpp::RawPacket* packet) {
//     // Parse the packet
//     pcpp::Packet parsedPacket(packet);
//
//     // Check if the packet has IPv4 and TCP layers
//     if (parsedPacket.isPacketOfType(pcpp::IPv4) && parsedPacket.isPacketOfType(pcpp::TCP)) {
//         const pcpp::TcpLayer* tcpLayer = parsedPacket.getLayerOfType<pcpp::TcpLayer>();
//
//         // Check for SYN scan
//         if (isSYNScan(tcpLayer)) {
//             std::cout << "Possible SYN scan detected!" << std::endl;
//             // Handle the detection as needed
//         }
//     }
// }
//
// void Port_Scanning_Detector::startListening() {
//     // Set a packet processing callback
//     m_PcapLiveDevice->startCapture(packetHandler, this);
//
//     // Sleep or perform other tasks while listening
//     // ...
//
//     // Stop capturing when done
//     m_PcapLiveDevice->stopCapture();
// }
//
//
//
// bool Port_Scanning_Detector::isSYNScan(const pcpp::TcpLayer* tcpLayer) {
//     // Check if the TCP packet is a SYN packet
//     return tcpLayer->getTcpHeader()->synFlag == 1 && tcpLayer->getTcpHeader()->ackFlag == 0;
// }
