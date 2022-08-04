#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iostream>

#include "../../include/libspm.h"




int readConfig(char* configFilePath)
{
    
    //debug 
    msg(level::DBG1, "Reading config file: %s", configFilePath);
    
    std::map<std::string, std::string> configMap;
    //open config file
    std::ifstream configFile;
    configFile.open(configFilePath);
    // read and parse config file
    std::string line;
    // Debug int
    int lnNum = 0;
    while (std::getline(configFile, line))
    {
        //Debug stuff
        lnNum++;
        msg(DBG3, "Parsing line %s : %s", (std::to_string(lnNum)).c_str(),line.c_str());

        // split line into key and value
        std::string key, value;
        key = line.substr(0, line.find_first_of("="));
        value = line.substr(line.find_first_of("=")+1, line.length());

        // add key and value to map
        configMap[key] = value;

        // store key and value in map
        configMap[key] = value;

    }
    // close config file
    configFile.close();

    msg(level::DBG2, "Parsing config data");
    // use the option on the soviet thing values
    if (configMap.count("ROOT") > 0)
    {
       ROOT = configMap["ROOT"];
        msg(DBG3, "ROOT : %s", ROOT.c_str());
    }
    if (configMap.count("MAIN_DIR") > 0)
    {
        MAIN_DIR = configMap["MAIN_DIR"];
    }
    if (configMap.count("DATA_DIR") > 0)
    {
        DATA_DIR = configMap["DATA_DIR"];
    }
    if (configMap.count("SPM_DIR") > 0)
    {
        SPM_DIR = configMap["SPM_DIR"];
    }
    if (configMap.count("LOG_DIR") > 0)
    {
        LOG_DIR = configMap["LOG_DIR"];
    }
    if (configMap.count("WORK_DIR") > 0)
    {
        WORK_DIR = configMap["WORK_DIR"];
    }
    if (configMap.count("BUILD_DIR") > 0)
    {
        BUILD_DIR = configMap["BUILD_DIR"];
    }
    if (configMap.count("MAKE_DIR") > 0)
    {
        MAKE_DIR = configMap["MAKE_DIR"];
    }
    if (configMap.count("REPOS") > 0)
    {
        std::string temp = "";
        std::string s = configMap["REPOS"]; 
        for(int i=0;i<s.length();++i){
            
            if(s[i]==' '){
                REPOS.push_back(temp);
                temp = "";
            }
            else{
                temp.push_back(s[i]);
            }
            
        }
        REPOS.push_back(temp);
        // debug stuff
        for (int i = 0;i < REPOS.size();i++)
        {
            msg(level::DBG3, "REPO : " + REPOS[i]);
        }
    }

    

    // If everything is okay 
    return 0;

}
/*
smol comment to remember how the config file works :

ROOT=/
MAIN_DIR=/var/cccp
DATA_DIR=/var/cccp/data
SPM_DIR=/var/cccp/spm
...etc...

REPOS= http://localhost:8080/ 
*/