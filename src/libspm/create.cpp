#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <sys/stat.h>

// class stuff
#include "../../include/libspm.hpp"


// parsing data and installing package
void soviet::package::createBinary(const std::string& binPath)
{
    std::string cmd_uncompress = soviet::format("tar -xf %s -C %s",packagePath.c_str(),vars.MAKE_DIR.c_str());
    //uncompressing <PName>.src.spm.tar.gz in PKG_DIR
    system(cmd_uncompress.c_str());

    // Reading spm file in MAKE DIR
    var_spm(soviet::format("%s/%s.spm",vars.MAKE_DIR.c_str(),name.c_str()));
    //making package
    make(format("%s/%s-%s",vars.MAKE_DIR.c_str(),name.c_str(),version.c_str()));
    // fancy output
    std::cout << "☭ Package built! ☭"<< "\n";
    //changing type to bin 
    type = "bin";
    //Get package locations
    get_locations();
    // Storing package data
    // Adding the locations to the package files , and the packages files to DATA_DIR
    store_spm(soviet::format("%s/%s.spm",vars.MAKE_DIR.c_str(),name.c_str()),soviet::format("%s/%s.spm",vars.BUILD_DIR.c_str(),name.c_str()));
    
    // Creating the tar.gz package archive
    std::string cmd_archive = soviet::format("( cd %s && tar -cf %s * )", vars.BUILD_DIR.c_str() , binPath.c_str());

    msg(DBG3, "Archiving package with %s", cmd_archive.c_str());

    system(cmd_archive.c_str());
    // cleaning build directory
    std::filesystem::remove_all(vars.BUILD_DIR);
    mkdir(vars.BUILD_DIR.c_str(), 0777);
}

