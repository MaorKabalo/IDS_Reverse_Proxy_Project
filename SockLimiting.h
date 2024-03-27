//
// Created by kali on 12/14/23.
//

#ifndef SOCKLIMITING_H
#define SOCKLIMITING_H

#include <map>
#include <iostream>
#include <limits>
//numbers are not importent can be change base on statistic of the server
#define MAX_SOCK 1000//std::numeric_limits<int>::max()  //1000
#define MAX_PER_SOCK 3//std::numeric_limits<int>::max() //5

class SockLimiting {
public:
    static bool isMaxSock();
    static bool add(std::string ip);
    static void remove(std::string ip);
private:
    static int _count;
    static std::map<std::string, int> _countPerSock;
};



#endif //SOCKLIMITING_H
