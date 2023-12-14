#include "SockLimiting.h"

bool SockLimiting::add(std::string ip)
{
    if(_count<MAX_SOCK&&_countPerSock.find(ip) == _countPerSock.end() || _countPerSock.find(ip)->second<MAX_PER_SOCK)
    {
        _count++;
        if(_countPerSock.find(ip) != _countPerSock.end())
            _countPerSock[ip]++;
        else
            _countPerSock[ip]=1;
    }
    return false;
}
