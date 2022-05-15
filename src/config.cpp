#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iostream>

int readConfig(std::string configFilePath)
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
        value = line.substr(line.find_first_of("="), line.length());
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

    // If everything is okay 
    return 0;

}