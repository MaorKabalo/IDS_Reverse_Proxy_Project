#ifndef IP_H
#define IP_H

#include <cstdint>
#include <string>
#include <cstring>
#include <cstdlib>

class IPTools {
public:
    static uint32_t IPv4StringToInt(const char* ipAddress);
    static std::string intToIPv4String(uint32_t ipAddress);
    static uint32_t ReverseIP(uint32_t ip);
};

#endif // IP_H
