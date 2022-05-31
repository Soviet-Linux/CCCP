#include <string>
#include <vector>
#include <iostream>

// class stuff
#include "../include/cccp.hpp"


// parsing data and installing package
void soviet::package::createBinary(const std::string& binPath)
{
    std::string USING_DIR;
    // chnaging uncompress and spm dir with package type
    if (type == "src") std::cout << "creating binary package from source package...";
    else {
        std::cout << "Package type not supported!" << std::endl;
        return;
    }

    std::string cmd_uncompress = soviet::format("tar -xf %s -C %s",packagePath.c_str(),USING_DIR.c_str());
    //uncompressing <PName>.src.spm.tar.gz in PKG_DIR
    system(cmd_uncompress.c_str());

    // Reading spm file in MAKE DIR
    open_spm(soviet::format("%s/%s.spm",USING_DIR.c_str(),name.c_str()));
    //making package
    make();
    // fancy output
    std::cout << "☭ Package built! ☭"<< "\n";
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

