#pragma once

#include <sys/types.h>
#include <vector>

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
    std::string download_info; // run 1
    std::string prepare_info; // Fuck run 
    std::string configure_info; // run 2
    std::string make_info; //run 3
    std::string test_info; //run 4
    std::string install_info; //run 5
    std::string special_info; // run 6
    //Installation locations data
    std::vector<std::string> locations;
};

void bin_spm (const std::string& in_path, const std::string& out_path);
void store_spm (const std::string& PPath,const std::string& BUILD_DIR,const std::string& out_path);
pkg_data open_spm (const std::string& PPath);