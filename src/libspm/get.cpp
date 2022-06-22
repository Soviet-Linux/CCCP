//#include <curl/curl.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
//include json lib
#include "../../lib/nlohmann/json.hpp"

 // class stuff
#include "../../include/libspm.hpp"

void soviet::package::get()
{
    packagePath = soviet::format("%s/%s.%s.spm.tar.gz",TMP_DIR.c_str(), name.c_str(), type.c_str());
    

    // I commented this part because the soviet system im working on right now doesnt support curl 
    // I will add it later whan the rest of the stuff is ready 
    
    // check if ALL_FILE exists
    if (access((ALL_FILE).c_str(),F_OK))
    {
        // This is the first tim is use a ''' do {...} while(...) ''' loop
        do
        {
            std::cout << "There are no package data file , do you want to download it ? [y/n]" << std::endl;
            std::cin >> type;
        }
        while(  type != "y" && type != "n");
        if (type == "n")
        {
            std::cout << "Exiting" << std::endl;
            return;
        }  
        else if (type == "y")
        {
            sync();
        } 
        else 
        {
            std::cout << "something went wrong" << std::endl;
        }

        
    }
    if (DEBUG) std::cout << "Loading " << ALL_FILE << std::endl;
    // verify if package exists
    // parse ALL_FILE and check if the package is there
    std::ifstream file_spm((ALL_FILE).c_str(), std::ios::in);
    std::stringstream buffer;
    buffer << file_spm.rdbuf();
    //parsing json data
    auto all_pkgs = json::parse(buffer.str());
    if (DEBUG) std::cout << "Parsing " << ALL_FILE << std::endl;
    // check if the package is there
    // this solution is pretty bad
    // TODO : improve it
    for (int i = 0; i < all_pkgs["all"].size(); i++)
    {
        std::string pkg_name = all_pkgs["all"][i]["name"];
        if (pkg_name  == name )
        {
            type = all_pkgs["all"][i]["type"];
            version = all_pkgs["all"][i]["version"];
            break;
        }
    }
    if (DEBUG) std::cout << "Package " << name << " " << version << " " << type << " found" << std::endl;
    // loop through REPOS
    for (int i = 0;i < REPOS.size();i++)
    {
        // get the url
        std::string repo = REPOS[i];
        std::string url = format("%sbase/%s/%s.%s.spm.tar.gz",repo.c_str(),type.c_str(),name.c_str(),type.c_str());
        
        std::string download_cmd = format("wget -O %s %s --no-check-certificate",packagePath.c_str(),url.c_str());

        if (!system(download_cmd.c_str())) {break;}

        /*
        CURL *curl;
        FILE *fp;
        CURLcode res;
        char outfilename[FILENAME_MAX] = "/tmp/test.test";
        curl = curl_easy_init();                                                                                                                                                                                                                                                           
        if (curl)
        {   
            fp = fopen(outfilename,"wb");
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            fclose(fp);

        }
        */

    
    }

    
}

/*
Some explanation about the ALL_FILE
Its a file where all packages names are stored
It is a json file
it's organized like this :

{
    "all" :
    [
        {
            "name" : "package1",
            "type" : "bin",
            "version" : "1.0.0",

        },
        {
            "name" : "package2",
            "type" : "src",
            "version" : "1.0.0",
        } 
    ]
}

*/
