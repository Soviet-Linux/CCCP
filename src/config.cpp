#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iostream>

#include "../include/cccp.hpp"

int soviet::readConfig(const std::string& configFilePath)
{
    std::map<std::string, std::string> configMap;
    //open config file
    std::ifstream configFile;
    configFile.open(configFilePath);
    // read and parse config file
    std::string line;
    while (std::getline(configFile, line))
    {
        // split line into key and value
        std::string key, value;
        key = line.substr(0, line.find_first_of("="));
        value = line.substr(line.find_first_of("=")+1, line.length());
        // print key and value
        std::cout << key << ": " << value << std::endl;
        // add key and value to map
        configMap[key] = value;

        // store key and value in map
        configMap[key] = value;
    }
    // close config file
    configFile.close();
    // use the option on the soviet thing values
    if (configMap.count("ROOT") > 0)
    {
        soviet::ROOT = configMap["ROOT"];
    }
    if (configMap.count("MAIN_DIR") > 0)
    {
        soviet::MAIN_DIR = configMap["MAIN_DIR"];
    }
    if (configMap.count("DATA_DIR") > 0)
    {
        soviet::DATA_DIR = configMap["DATA_DIR"];
    }
    if (configMap.count("SPM_DIR") > 0)
    {
        soviet::SPM_DIR = configMap["SPM_DIR"];
    }
    if (configMap.count("LOG_DIR") > 0)
    {
        soviet::LOG_DIR = configMap["LOG_DIR"];
    }
    if (configMap.count("WORK_DIR") > 0)
    {
        soviet::WORK_DIR = configMap["WORK_DIR"];
    }
    if (configMap.count("BUILD_DIR") > 0)
    {
        soviet::BUILD_DIR = configMap["BUILD_DIR"];
    }
    if (configMap.count("MAKE_DIR") > 0)
    {
        soviet::MAKE_DIR = configMap["MAKE_DIR"];
    }
    if (configMap.count("REPOS") > 0)
    {
        std::string temp = "";
        std::string s = configMap["REPOS"]; 
        for(int i=0;i<s.length();++i){
            
            if(s[i]==' '){
                soviet::REPOS.push_back(temp);
                temp = "";
            }
            else{
                temp.push_back(s[i]);
            }
            
        }
        REPOS.push_back(temp);
        for (int i = 0;i < REPOS.size();i++)
        {
            std::cout << REPOS[i] << std::endl;
        }
    }

    

    // If everything is okay 
    return 0;

}