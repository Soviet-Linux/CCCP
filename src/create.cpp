#include <string>
#include <vector>
#include <iostream>

// class stuff
#include "../include/cccp.hpp"


// parsing data and installing package
void soviet::package::create(const std::string& binPath)
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
    //making package
    make();
    // fancy output
    std::cout << "☭ Package built"<< "\n";
    //changing type to bin 
    type = "bin";
    //Get package locations
    get_locations();
    rename(soviet::format("/tmp/%s.tmp.spm"),name.c_str()),soviet::format("%s/%s.spm",BUILD_DIR.c_str(),name.c_str());

    // Storing package data
    // Adding the locations to the package files , and the packages files to DATA_DIR
    store_spm(soviet::format("%s/%s.spm",BUILD_DIR.c_str(),name.c_str()),soviet::format("%s/%s.spm",BUILD_DIR.c_str(),name.c_str()));
    
    // Creating the tar.gz package archive
    std::string cmd_archive = soviet::format("( cd %s && tar -cf %s * ", BUILD_DIR.c_str() , binPath.c_str());

    if (DEBUG) std::cout << cmd_archive << std::endl;

    system(cmd_archive.c_str());
    // cleaning build directory
    system(("rm -rf " + BUILD_DIR + "*").c_str());
}

