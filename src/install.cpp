#include <fstream>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <filesystem>


// class stuff
#include "../include/cccp.hpp"





/*
In this file we are installing source or binary packages.
Source packages are archive files containing the source code of the package ,the post install script and an .spm file for the install commands.
Binary packages are archive files containing the compiled binary files of the package , the post install script and an .spm file for the install commands.
*/


// parsing data and installing package
void soviet::package::install()
{
    std::string USING_DIR;
    // chnaging uncompress and spm dir with package type
    if (type == "src") USING_DIR = MAKE_DIR;
    else if (type == "bin") USING_DIR = BUILD_DIR;
    else {
        std::cout << "Package type not supported" << std::endl;
        return;
    }

    // checking if package is already installed
    if (check())
    {
        std::cout << "Package is already installed, reinstalling" << std::endl;
        // removing the package
        purge();

    }
    std::string cmd_uncompress = soviet::format("tar -xf %s -C %s",packagePath.c_str(),USING_DIR.c_str());
    //uncompressing <PName>.src.spm.tar.gz in PKG_DIR
    system(cmd_uncompress.c_str());

    // Reading spm file in MAKE DIR
    open_spm(soviet::format("%s/%s.spm",USING_DIR.c_str(),name.c_str()));

    // Checking dependencies
    //This dependencies if;else system is deprecated and will be removed in the future
    if (check_dependencies())
    {
        std::cout << "Dependency check passed"<< "\n";     
    }
    else
    {
        std::cout << "Dependency check failed"<< "\n";
        // TODO: ADD THE DEPENDENCIES STUFF HERE
        exit(1);
    }

    // building the package and getting locations if its a source package
    if (type == "src" ) 
    {
        //making package
        make();
        // fancy output
        std::cout << "☭ Package built"<< "\n";
        //Get package locations
        get_locations();
        rename(soviet::format("/tmp/%s.tmp.spm"),name.c_str()),soviet::format("%s/%s.spm",BUILD_DIR.c_str(),name.c_str());
    }
    // Moving built binaries to their install location on the system
    move_binaries();

    //executing post installation scripts
    if (!special_info.empty()) 
    {
        system((BUILD_DIR + special_info).c_str());  
    }
    else 
    {
        std::cout << "No post installation scripts found" << "\n";
    }
    // Storing package data
    // Adding the locations to the package files , and the packages files to DATA_DIR
    store_spm(soviet::format("%s/%s.spm",BUILD_DIR.c_str(),name.c_str()),dataSpmPath);

    //adding the package to the data file
    add_data();
    // Cleaning everything
    system(("rm -rf " + BUILD_DIR + "*").c_str());

}


