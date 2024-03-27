#include "SockLimiting.h"

int SockLimiting::_count=0;
std::map<std::string, int> SockLimiting::_countPerSock;

bool SockLimiting::add(std::string ip)
{
    if(isMaxSock())
        return false;
    if(_count<MAX_SOCK&&_countPerSock.find(ip) == _countPerSock.end() || _countPerSock.find(ip)->second<MAX_PER_SOCK)
    {
        _count++;
        if(_countPerSock.find(ip) != _countPerSock.end())
            _countPerSock[ip]++;
        else
            _countPerSock[ip]=1;
        return true;
    }
    return false;
}

bool SockLimiting::isMaxSock()
{
    return _count == MAX_SOCK;
}

void SockLimiting::remove(std::string ip) {
    _countPerSock.erase(ip);
}