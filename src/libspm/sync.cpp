#include <curl/curl.h>
#include <iostream>
#include <string>
#include <unistd.h>

// class stuff
#include "../../include/libspm.hpp"

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

void soviet::sync ()
{
    // This also uses cURL

    
    for (int i = 0; i < REPOS.size(); i++)
    {
        std::string repo = REPOS[i];
        std::string all_url = format("%s/all.json",repo.c_str());
        FILE *data = fopen(ALL_FILE.c_str(),"wb");
        // if not , download it
        std::cout << "Downloading " << ALL_FILE << " from " << all_url << std::endl;

        
        CURL *curl;
        FILE *fp;
        CURLcode res;
        curl = curl_easy_init();
        if (curl) {
            std::cout << "Downloading " << all_url << std::endl;
            fp = fopen(ALL_FILE.c_str(),"wb");
            curl_easy_setopt(curl, CURLOPT_URL, all_url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            res = curl_easy_perform(curl);
            /* always cleanup */
            curl_easy_cleanup(curl);
            fclose(fp);
            break;
        }
        // Idk why but it doent work
        

    }
    
    
    


    
}