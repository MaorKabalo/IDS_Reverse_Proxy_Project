//
// Created by kali on 1/9/24.
//
#define OK 0//IP is fine
#define MALICIOUS 1//IP is MALICIOUS. need to block it
#define ERROR 2//error in the proccess of chaking the IP dont accept and dont block
#include "IPTools.h"

#ifndef CHECKIP_H
#define CHECKIP_H
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>


class checkIP : public IPTools{

    public:
        static int CheckIP(const std::string &ipAddress);

    private:
        static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);
};



#endif //CHECKIP_H
