#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

#include <curl/curl.h>

// class stuff
#include "../../include/libspm.h"
#include "../../include/utils.h"



int progress_func(void* ptr, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded)
{
    

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

    //declare the progressmeter
    char bar[50];

    // create the "meter"
    int ii=0;
    // part  that's full already
    for ( ; ii < (dotz - 1);ii++) {
        strcat(bar,"=");
    }
    strcat(bar,">");
    // remaining part (spaces)
    for ( ; ii < totaldotz;ii++) {
        strcat(bar," ");
    }
    // and back to line begin - do not forget the fflush to avoid output buffering problems!
    printf(" %f [%s]\r",fractiondownloaded ,bar);
    fflush(stdout);
    // if you don't return 0, the transfer will be aborted - see the documentation
    return 0; 
}
int downloadRepo(const char* url_path,const char* file_path)
{
    for (int i = 0;i < REPO_COUNT;i++)
    {
        // get the url
        char* repo = REPOS[i];
        printf("repo is %s\n",repo);
        char* url = format("%s/%s",repo,url_path);
        msg(INFO, "Downloading %s", url);
        
        if (downloadFile(url,file_path) == 0)
        {
            return 0;
        }
    
    }
    return 1;
    
} 
int downloadFile(const char* url,const char* file_path)
{
    CURL *curl;
    FILE *fp;
    CURLcode res;
    curl = curl_easy_init();      
    msg(DBG3,"curl_easy_init() returned %p",curl);                                                                                                                                                                                                                                                     
    if (!curl)
    {
        msg(ERROR,"curl_easy_init() failed");
        return -1;  
    } 

    
    fp = fopen(file_path,"wb");
    curl_easy_setopt(curl, CURLOPT_URL, url);
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

    if (res != CURLE_OK)
    {
        return -1;
    }
    return 0;

}


bool is_in_repo(CURL* session)
{
    CURLcode curl_code;
    curl_code = curl_easy_perform (session);
    long http_code = 0;
    curl_easy_getinfo (session, CURLINFO_RESPONSE_CODE, &http_code);
    if (http_code == 200 && curl_code != CURLE_ABORTED_BY_CALLBACK)
    {
        printf("found in repo\n");
        return true;
    }
    else
    {
        return false;
    }
}

