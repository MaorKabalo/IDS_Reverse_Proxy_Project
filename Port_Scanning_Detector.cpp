//
// Created by kali on 2/12/24.
//

#include "Port_Scanning_Detector.h"
#include <iostream>
#include <utility>
#include <thread>
#include <netinet/in.h>
#include <chrono>
#include <exception>


int packetCount = 0;


void Port_Scanning_Detector::extractPorts() {
    std::ifstream file(NMAP_PORTS_TXT);

    std::string line;
    while (std::getline(file, line)) {
        m_Ports.push_back(std::stoi(line));
    }

    file.close();

    std::cout << "Ports Extracted" << std::endl;
}

void Port_Scanning_Detector::onPacketArrives(RawPacket* packet, PcapLiveDevice* dev, void* cookie) {
    // Parse and process the packet
    Packet parsedPacket(packet);

    ProtocolType transportProtocol = parsedPacket.getLayerOfType<TcpLayer>()->getProtocol();
    auto* tcpLayer = parsedPacket.getLayerOfType<TcpLayer>();

    if (transportProtocol == TCP){
        uint16_t destPort = ntohs(tcpLayer->getTcpHeader()->portDst);

        //std::cout << destPort << std::endl;

        if(destPort == 8888) {
            std::cout << "GOT HERE!!!" << std::endl;  //1 message sent, 5 prints
        }
    }


}


Port_Scanning_Detector::Port_Scanning_Detector(std::string interfaceName) {

    // std::thread thread(&Port_Scanning_Detector::extractPorts, this);
    // thread.detach();


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

