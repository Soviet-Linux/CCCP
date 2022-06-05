#include <curl/curl.h>
#include <iostream>
#include <string>
#include <unistd.h>

// class stuff
#include "../../include/cccp.hpp"

void soviet::sync ()
{
    // This also uses cURL
    /*
    for (int i = 0; i < REPOS.size(); i++)
    {
        std::string repo = REPOS[i];
        std::string all_url = format("%s/all.json",repo.c_str());
        FILE *data = fopen(ALL_FILE.c_str(),"wb");
        // if not , download it
        std::cout << "Downloading " << ALL_FILE << " from " << all_url << std::endl;
        curl_global_init(CURL_GLOBAL_ALL);
        CURL *curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, all_url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        // write the data to ALL_FILE
        std::ofstream file_spm((ALL_FILE).c_str(), std::ios::out);
        file_spm << data;
        file_spm.close();
    }
    */
}