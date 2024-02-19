//
// Created by kali on 2/12/24.
//

#ifndef PORT_SCANNING_DETECTOR_H
#define PORT_SCANNING_DETECTOR_H

#include <Packet.h>
#include <IPv4Layer.h>
#include <TcpLayer.h>
#include <PcapLiveDeviceList.h>
#include <PcapLiveDevice.h>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <cstdint>
#include <thread>
#include <mutex>

#define NMAP_PORTS_TXT "nmapDefault.txt"

using namespace pcpp;

class Port_Scanning_Detector {
public:

    explicit Port_Scanning_Detector(std::string interfaceName);
    ~Port_Scanning_Detector();

    void ListenForSYNScanAttack() const;

    std::vector<std::string> mMalicousIPs;

private:

    std::string m_InterfaceName;
    PcapLiveDevice* m_PcapLiveDevice;
    std::vector<uint16_t> m_Ports;

    static std::mutex countPacketMutex;

    static void onPacketArrives(RawPacket* packet, PcapLiveDevice* dev, void* cookie);
    void extractPorts();


};



#endif //PORT_SCANNING_DETECTOR_H
