#include <fstream>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <filesystem>
#include <sys/stat.h>


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
    /* All these variables are bad.*/
    // file where we temporarily store the spm data
    std::string temp_file = soviet::format("/tmp/%s.spm.tmp",name.c_str());
    //location spm file in build dir
    std::string spm_build = soviet::format("%s/%s.spm",BUILD_DIR.c_str(),name.c_str());


    std::string USING_DIR;
    // chnaging uncompress and spm dir with package type
    if (type == "src") USING_DIR = MAKE_DIR;
    else if (type == "bin") USING_DIR = BUILD_DIR;
    else {
        std::cout << "Package type not supported" << std::endl;
        return;
    }

    std::string cmd_uncompress = soviet::format("tar -xf %s -C %s ",packagePath.c_str(),USING_DIR.c_str());
    // if debug is on , print the command
    if (soviet::DEBUG) std::cout << cmd_uncompress << std::endl;
    //uncompressing <PName>.src.spm.tar.gz in PKG_DIR
    system(cmd_uncompress.c_str());

    //debug
    if(soviet::DEBUG) std::cout << soviet::format("%s/%s.spm",USING_DIR.c_str(),name.c_str()) << std::endl;
    // Reading spm file in MAKE DIR
    var_spm(soviet::format("%s/%s.spm",USING_DIR.c_str(),name.c_str()));
    // checking if the package is already installed
    if (soviet::DEBUG) std::cout << "checking if "<< dataSpmPath << " exists :  " << access(dataSpmPath.c_str(), F_OK) << "\n";
    // this access function is weird , the return is 0 if it works and -1 if it doesnt 
    if (!access(dataSpmPath.c_str(), F_OK))
    {
        std::cout << "Package is already installed , reinstalling." << std::endl;
    }
    else
    {
        if(soviet::DEBUG) std::cout << "Package is not installed , installing." << std::endl;
    }
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
        //moving temporary spm files to build dir to match bin package look
        rename(temp_file.c_str(),spm_build.c_str());
        if (soviet::DEBUG) std::cout << "Spm file moved" << "\n";
        // Using the C function because it's better
        remove(temp_file.c_str());
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
    store_spm(spm_build,dataSpmPath);

    //adding the package to the data file
    add_data();

    // Cleaning everything
    // Again im forced to use this fucking std::filesystem
    std::filesystem::remove_all(BUILD_DIR);
    // recreating the dir 
    mkdir(BUILD_DIR.c_str(), 0777);
    /* 
    The solution i used in the code above is bad 
    If you have an idea , do it 
    The goal is to remove all the content of BUILD_DIR 
    Without using complicated stuff
    */


}


