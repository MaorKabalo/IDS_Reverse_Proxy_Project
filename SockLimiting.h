//
// Created by kali on 12/14/23.
//

#ifndef SOCKLIMITING_H
#define SOCKLIMITING_H

#include <map>
#include <iostream>
//numbers are not importent can be change base on statistic of the server
#define MAX_SOCK 1000
#define MAX_PER_SOCK 5

class SockLimiting {
public:
    //static bool isMaxSock() const;
    static bool add(std::string ip);
private:
    static int _count;
    static std::map<std::string, int> _countPerSock;
};



#endif //SOCKLIMITING_H
