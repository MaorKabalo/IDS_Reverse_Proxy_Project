/*
 * Port_Scanning_Detector.cpp
 * 
 * Created by kali on 2/12/24.
 * 
 * This class implements a Port Scanning Detector that monitors incoming packets
 * on a specified network interface and detects potential port scanning attacks.
 * If a port scan is detected, the corresponding IP address is blocked.
 */

#include "Port_Scanning_Detector.h"
#include "IPTools/BlockIP.h"

#define PORT 100     // Threshold for port scans
#define PER_MS 15    // Time window in milliseconds

// Static member initialization
int Port_Scanning_Detector::portsScannedCount = 0;
std::unordered_set<uint16_t> Port_Scanning_Detector::m_Ports;
std::chrono::steady_clock::time_point Port_Scanning_Detector::lastPacketTime;
std::unordered_set<std::string> Port_Scanning_Detector::mMalicousIPs;

// Mutexes for thread safety
std::mutex countPortMutex;
std::mutex mMalicousIPsMutex;
std::mutex lastPacketTimeMutex;

int countPort = 0;  // Counter for tracking scanned ports

/**
 * Extracts known ports from the NMAP port list file.
 * Reads ports from "NMAP_PORTS_TXT" and stores them in an unordered set.
 */
void Port_Scanning_Detector::extractPorts() {
    std::ifstream file(NMAP_PORTS_TXT);
    uint16_t port;
    while (file >> port) {
        m_Ports.insert(port);
    }
}

/**
 * Callback function triggered when a packet arrives.
 * 
 * @param packet The raw packet received
 * @param dev The live capture device
 * @param cookie Additional user data (unused)
 * 
 * This function processes incoming packets to identify potential port scanning behavior.
 * It extracts the source IP address and destination port from the packet, updates scan counters,
 * and determines whether the activity exceeds predefined thresholds for port scans.
 */
void Port_Scanning_Detector::onPacketArrives(RawPacket* packet, PcapLiveDevice* dev, void* cookie) {
    // Parse the raw packet into structured layers
    Packet parsedPacket(packet);
    auto* tcpLayer = parsedPacket.getLayerOfType<TcpLayer>();
    auto* ipLayer = parsedPacket.getLayerOfType<IPv4Layer>();

    // Ensure both TCP and IP layers exist before proceeding
    if (!tcpLayer || !ipLayer) return;

    // Extract source IP and destination port from the packet
    uint32_t sourceIP = ipLayer->getIPv4Header()->ipSrc;
    uint16_t destPort = ntohs(tcpLayer->getTcpHeader()->portDst);

    // Check if the destination port is among the monitored ports and update counter safely
    {
        std::lock_guard<std::mutex> lock(countPortMutex);
        if (m_Ports.find(destPort) != m_Ports.end()) {
            countPort++;
        }
    }

    // Calculate time difference since last recorded packet arrival
    auto currentTime = std::chrono::steady_clock::now();
    auto timeDifference = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastPacketTime).count();

    // Evaluate whether the scan count threshold is exceeded within the specified time window
    if (timeDifference > PER_MS) {
        {
            std::lock_guard<std::mutex> lock(countPortMutex);
            if (countPort >= PORT) {
                std::cout << "Port Scanning Detected!" << std::endl;
                BlockIP::blockIP(IPTools::intToIPv4String(sourceIP).c_str());
            }
        }

        // Reset counters safely using mutex locks to prevent race conditions
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

/**
 * Constructor: Initializes the detector on a specific network interface.
 * 
 * @param interfaceName Name of the network interface to monitor.
 */
Port_Scanning_Detector::Port_Scanning_Detector(std::string interfaceName) {
    std::thread thread(&Port_Scanning_Detector::extractPorts);
    thread.detach();
    
    m_InterfaceName = std::move(interfaceName);
    PcapLiveDeviceList& devList = PcapLiveDeviceList::getInstance();
    int interfaceIndex = -1;

    // Find the specified network interface
    int i = 0;
    for (const PcapLiveDevice* dev : devList.getPcapLiveDevicesList()) {
        if (dev->getName() == m_InterfaceName) {
            interfaceIndex = i;
        }
        i++;
    }

    // Attempt to open the network interface
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
}

/**
 * Destructor: Closes the network interface if open.
 */
Port_Scanning_Detector::~Port_Scanning_Detector() {
    if (m_PcapLiveDevice != nullptr) {
        m_PcapLiveDevice->close();
    }
}

/**
 * Starts listening for potential SYN scan attacks on the network interface.
 */
void Port_Scanning_Detector::ListenForSYNScanAttack() const {
    std::cout << "Port Scanner Listening on interface: " << m_PcapLiveDevice->getName() << std::endl;
    m_PcapLiveDevice->startCapture(onPacketArrives, nullptr);
    std::cin.get(); // Wait for user input to stop capturing
}
