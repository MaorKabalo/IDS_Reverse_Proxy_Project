//
// Created by kali on 2/12/24.
//
//
// #ifndef PORT_SCANNING_DETECTOR_H
// #define PORT_SCANNING_DETECTOR_H
//
// #include <Packet.h>
// #include <IPv4Layer.h>
// #include <TcpLayer.h>
// #include <PcapLiveDeviceList.h>
// #include <PcapLiveDevice.h>
//
// using namespace pcpp;
//
// class Port_Scanning_Detector {
// public:
//     explicit Port_Scanning_Detector(std::string  interfaceName);
//     ~Port_Scanning_Detector();
//
//     void startListening();
//
// private:
//     std::string m_InterfaceName;
//     pcpp::PcapLiveDevice* m_PcapLiveDevice;
//
//     void packetHandler(pcpp::RawPacket* packet);
//     static bool isSYNScan(const pcpp::TcpLayer* tcpLayer);
// };
//
//
//
// #endif //PORT_SCANNING_DETECTOR_H
