//
// Created by kali on 1/9/24.
//

#include "checkIP.h"
#include<iostream>


int checkIP::CheckIP(const std::string &ipAddress)
{
    std::string apiKey = "5437744fa0729c5504e7e497dd5e6e07306a4a445a7a1fafccb2f13f5f5feedb609d57cf496a151e";
    std::string url = "https://api.abuseipdb.com/api/v2/check?ipAddress=" + ipAddress;// + "&maxAgeInDays=90&verbose=true";
    std::string key = "Key: " + apiKey;

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, key.c_str());

    CURL *curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        std::string response_string;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

        CURLcode response = curl_easy_perform(curl);
        if (response != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(response));
            return ERROR;
        }

        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

        curl_easy_cleanup(curl);

        if (http_code == 200)
        {
            nlohmann::json response_json = nlohmann::json::parse(response_string);
            bool isWhitelisted = response_json["data"]["isWhitelisted"];
            //std::cout<<response_json["data"]["isWhitelisted"]<<std::endl;
            return isWhitelisted?OK:MALICIOUS;
        }
    }

    return ERROR;
}

size_t checkIP::write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)buffer, size * nmemb);
    return size * nmemb;
}