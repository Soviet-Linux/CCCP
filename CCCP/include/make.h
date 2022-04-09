#pragma once

// I had to include this but I don't know why
#include <sys/types.h>

#include "../include/nlohmann/json.hpp"

//A struct to hold the installation data
//TODO : explain this better
struct location
{
    //The file to be installed
    std::string file;
    // Where to install it
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
    //If the type is bin or local , build_info and download_info will be empty 
    //I havent found a better solution that doesn't require a lot of stuff
    std::string build_info;
    std::string download_info;
    //just for local packages , the location of the source archive
    std::string archive;
    //Installation locations data
    std::vector<location> install_info;
};

//Prototypes
void make_pkg(const std::string& PName, const std::string& build_info, const std::string& WORK_DIR);
void download_pkg( const std::string& download_info, const std::string& WORK_DIR);
pkg_data open_spm (const std::string& PPath);
void move_binaries(const std::string& BUILD_DIR ,const std::vector<location>& install_info);
int check_dependencies (const std::vector<std::string>& dependencies, const std::string& DATA_DIR); 
void bin_spm (const std::string& in_path, const std::string& out_path);
