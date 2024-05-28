#include "IPTools.h"
#include <cstring>

#include <cstring> // For strtok
#include <cstdlib> // For atoi
#include "IPTools.h"

uint32_t IPTools::IPv4StringToInt(const char* ipAddress) {
    char* ipAddressCopy = strdup(ipAddress); // Make a copy
    char* octet = strtok(ipAddressCopy, ".");
    uint32_t ipInt = 0;
    int shift = 24;

    while (octet != nullptr && shift >= 0) {
        uint32_t octetInt = atoi(octet);
        if (octetInt > 255) {
            // Handle invalid octet value
            free(ipAddressCopy);
            return 0; // Or handle the error appropriately
        }
        ipInt |= (octetInt << shift);
        octet = strtok(nullptr, ".");
        shift -= 8;
    }

    free(ipAddressCopy);
    return ipInt;
}


std::string IPTools::intToIPv4String(uint32_t ipAddress) {
    uint8_t octet1 = (ipAddress >> 24) & 0xFF;
    uint8_t octet2 = (ipAddress >> 16) & 0xFF;
    uint8_t octet3 = (ipAddress >> 8) & 0xFF;
    uint8_t octet4 = ipAddress & 0xFF;

    return std::to_string(octet4) + "." + std::to_string(octet3) + "." +
           std::to_string(octet2) + "." + std::to_string(octet1);
}

uint32_t IPTools::ReverseIP(uint32_t ip) {
    return ((ip & 0xFF) << 24) | ((ip >> 8 & 0xFF) << 16) |
           ((ip >> 16 & 0xFF) << 8) | ((ip >> 24) & 0xFF);
}
