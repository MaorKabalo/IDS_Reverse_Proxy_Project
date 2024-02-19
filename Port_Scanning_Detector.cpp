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
    pcpp::Packet parsedPacket(packet);
    std::cout << "Packet captured. Payload size: " << parsedPacket.getRawPacket()->getRawDataLen() << " bytes" << std::endl;
}


Port_Scanning_Detector::Port_Scanning_Detector(std::string interfaceName) {
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

    // Start capturing with the callback function
    m_PcapLiveDevice->startCapture(onPacketArrives, nullptr);


    std::this_thread::sleep_for(std::chrono::seconds(10));

    // Wait for user input or some other event to stop capturing
    // std::cout << "Press Enter to stop capturing..." << std::endl;
    // std::cin.get();

    // Stop capturing and close the interface
    m_PcapLiveDevice->stopCapture();
    m_PcapLiveDevice->close();
    free(m_PcapLiveDevice);
}

