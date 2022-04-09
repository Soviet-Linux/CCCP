#pragma once

#include <sys/types.h>
#include "../include/nlohmann/json.hpp"

//A struct to hold the installation data
//TODO : explain this better
struct location
{
    std::string file;
    std::string destination;
    
};

// struct defining all infomation needed to install a package
struct pkg_data
{
    //Package name
    std::string name;
    //Package type (binary, source, etc)
    std::string type;
    //Package version
    std::string version;
    //a list of package dependencies
    std::vector<std::string> dependencies;
    //If the type is bin , build_info and download_info will be empty 
    //I havent found a better solution that doesn't require a lot of stuff
    std::string build_info;
    std::string download_info;
    //Installation locations data
    std::vector<location> install_info;
};

//Prototypes
void make_pkg(const std::string& PName, const std::string& download_info, const std::string& build_info, const std::string& CURRENT_DIR);
int check_dependencies(const nlohmann::basic_json<>&, const std::string& DATA_DIR);
