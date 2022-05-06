#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>

#include "../lib/pkg_data.h"


// Creating a binary package from a .spm file
void create_binary(const std::string &PName,const std::string &PKG_DIR)
{
    // PKG file location
    std::string PPath = PKG_DIR + PName + ".spm";
    // checking if the package file exists 
    if (!access(PPath.c_str(), F_OK))
    {
        std::cout << "Package " << PName << " already exists. Terminating.\n";
        exit(1);
    }
    // temp spm file location ( I know , this is not very good , if you are very intelligent maybe you could change it :)
    //  TODO: change this to a better location
    std::string temp_path = "/tmp/temp.spm";

    std::cout << "Processing package " << PName << "\n";
    // Getting package data from .spm file
    const pkg_data &pkg_info = open_spm(PPath);
    if (pkg_info.type == "src")
    {
        // downloading package source into the work directory
        const std::string &download_cmd = "( cd " + MAKE_DIR + " && " + pkg_info.download_info + " )";

        std::cout << download_cmd << "\n";
        system(download_cmd.c_str());
    }
    else if (pkg_info.type == "local")
    {
        // unpacking the sources archive
        std::string cmd_source = "tar -xf " + SRC_DIR + PName + "*" + " -C " + WORK_DIR + "sources/";
        if (DEBUG) std::cout << cmd_source << "\n";
        system(cmd_source.c_str());
    }

    // making the package from source
    make_pkg(pkg_info, MAKE_DIR, BUILD_DIR,LOG_DIR,TESTING);
    std::cout << "☭ Package built"<< "\n";

    // adding locations and other thing to spm file
    bin_spm(PPath, temp_path);
    store_spm(temp_path, BUILD_DIR, BUILD_DIR + PName + ".spm");
    
    // Creating the tar.gz package archive
    std::string cmd_archive = "( cd " + BUILD_DIR + " && tar -cf " + BIN_DIR + PName + ".tar.gz * )";

    if (DEBUG) logger.Print<Soviet::DEBUG>("%s",cmd_archive.c_str());

    system(cmd_archive.c_str());
    // cleaning build directory
    system(("rm -rf " + BUILD_DIR + "*").c_str());
}

