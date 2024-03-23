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
#include <unordered_set>
#include <utility>
#include <netinet/in.h>
#include <chrono>
#include <exception>
#include <set>
#include "IPTools.h"

#define NMAP_PORTS_TXT "nmapDefault.txt"

using namespace pcpp;

class Port_Scanning_Detector {
public:

    Port_Scanning_Detector(std::string interfaceName);
    ~Port_Scanning_Detector();

    void ListenForSYNScanAttack() const;

    static std::unordered_set<std::string> mMalicousIPs;

private:

    std::string m_InterfaceName;
    PcapLiveDevice* m_PcapLiveDevice;

    static std::unordered_set<uint16_t> m_Ports;
    static int portsScannedCount;
    static std::chrono::steady_clock::time_point lastPacketTime;

    static void onPacketArrives(RawPacket* packet, PcapLiveDevice* dev, void* cookie);
    static void extractPorts();


};



#endif //PORT_SCANNING_DETECTOR_H
