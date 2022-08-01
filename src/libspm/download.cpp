#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <curl/curl.h>

// class stuff
#include "../../include/libspm.hpp"

int progress_func(void* ptr, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded)
{
    std::string barString = "";

    // ensure that the file to be downloaded is not empty
    // because that would cause a division by zero error later on
    if (TotalToDownload <= 0.0) {
        return 0;
    }

    // how wide you want the progress meter to be
    int totaldotz=40;
    double fractiondownloaded = NowDownloaded / TotalToDownload;
    // part of the progressmeter that's already "full"
    int dotz = (int) round(fractiondownloaded * totaldotz);

    // create the "meter"
    int ii=0;
    barString += soviet::format("%3.0f%% [",fractiondownloaded*100);
    // part  that's full already
    for ( ; ii < dotz;ii++) {
        barString += "=";
    }
    // remaining part (spaces)
    for ( ; ii < totaldotz;ii++) {
        barString += " ";
    }
    // and back to line begin - do not forget the fflush to avoid output buffering problems!
    barString += "]";
    soviet::msg(soviet::DOWNLOAD, "%s", barString.c_str());
    // if you don't return 0, the transfer will be aborted - see the documentation
    return 0; 
}
int soviet::downloadRepo(const std::string& url_path,const std::string& file_path)
{
    for (int i = 0;i < vars.REPOS.size();i++)
    {
        // get the url
        std::string repo = vars.REPOS[i];
        std::string url = format("%s/%s",repo.c_str(),url_path.c_str());
        msg(INFO, "Downloading %s", url.c_str());
        
        downloadFile(url,file_path);
    
    }
    return 0;
} 
int soviet::downloadFile(const std::string& url,const std::string file_path)
{
    CURL *curl;
    FILE *fp;
    CURLcode res;
    curl = curl_easy_init();      
    msg(DBG3,"curl_easy_init() returned %p",curl);                                                                                                                                                                                                                                                     
    if (curl)
    {   
        fp = fopen(file_path.c_str(),"wb");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        // Internal CURL progressmeter must be disabled if we provide our own callback
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, false);
        // Install the callback function
        msg(DBG3,"launching progress func");
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_func); 
        
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
        printf("\n");
    }
    else {
        msg(ERROR,"curl_easy_init() failed");
    }
    return 0;
}

