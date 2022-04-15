#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <ios>
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <vector>
#include <algorithm>
#include <filesystem>


#include "../include/main.h"
#include "../include/make.h"

//The filesystem root
const std::string ROOT = "/home/paulk/Desktop/soviet-lfs-rebuild/";
//For real use it must be set to "/"

//the dir where we will be building the packages and downoading the sources
const std::string WORK_DIR = ROOT + "/var/cccp/work/";
//the dir where the package file are storedds
const std::string PKG_DIR = ROOT + "/var/cccp/pkg/";
//the dir where the data is stored
const std::string DATA_DIR = ROOT + "/var/cccp/data/";
//where the sources are stored for local packages
const std::string SRC_DIR = ROOT + "/var/cccp/src/";
//Main function
int main (int argc, char *argv[]) 
{
    //verifying if the user has entered arguments
    if (argc < 2) {
        std::cout << "No arguments given. Terminating.\n";
        exit(1);
    }

    std::string option = argv[1];

    //parsing argument 
    // TODO: use a switch statement here
    if (option.substr(0,2) == "--") {
        if (option == "--install") {
            install_package(argv[2]);
        }
        else if (option == "--create") {

            create_binary(argv[2]);
        }
        else if (option == "--binary") {
            install_binary(argv[2]);
        }
        else if (option == "--test")
        {
            std::cout << "Testing\n";
        }
        
    }
    else {
        //Print the correct usage
        std::cout << "No option given\n";
        std::cout << "To install a source package : \n";
        std ::cout << "Usage: cccp --install <package_name>\n\n";
        std::cout << "To create a binary package from source package package : \n";
        std ::cout << "Usage: cccp --create <package_name>\n\n";
        std::cout << "To install a binary package : \n";
        std ::cout << "Usage: cccp --binary <package_name>\n\n";
        exit(1);
    }  
    
    //Returning 0 means the program ran successfully
    // 
    return 0;
}



//parsing data and installing package
void install_package (const std::string& PName)
{
    std::cout << "processing package " << PName << "\n"; 
    //Getting the package data from the spm file
    const pkg_data& pkg_info = open_spm(PKG_DIR + PName + ".spm"); 

    //Checking dependencies
    if (check_dependencies(pkg_info.dependencies, DATA_DIR))
    {
        std::cout << "dependencies are ok" << "\n";
        if (pkg_info.type == "src")
        {
            //downloading package source into the work directory
            download_pkg(pkg_info.download_info, WORK_DIR);
        }
        else if (pkg_info.type == "local") {
            std::string cmd_archive = "tar -xf " + SRC_DIR + PName + "*" + " -C " + WORK_DIR + "sources/";
            std::cout << cmd_archive << "\n";
            system(cmd_archive.c_str());
        
        }
        
        //making the package from source
        make_pkg(PName, pkg_info.build_info, WORK_DIR);
        std::cout << "package built" << "\n";
    }
    else {
        std::cout << "dependencies are not ok" << "\n";
        //TODO: ADD THE DEPENDENCIES STUFF HERE

    }
     
    //Moving built binaries to their install location on the system
    //TDOD : for the release we should change a BUILD_DIR
    move_binaries(WORK_DIR + "build/",ROOT);

    // TODO: write pkg infos to a packages database
    //

}

//this function installs a binary package
int install_binary(const std::string& PName)
{
    //Creating a random temporay dir name
    std::string TMP_DIR = "/tmp/" + std::to_string((rand() % 100000 + 1 )) + "/";
    //Uncompressing the binary package into the temorary dir
    system(("tar -xf " + std::filesystem::current_path().string() +"/"+ PName + ".tar.gz -C" + TMP_DIR ).c_str());
    //Reading package data from .spm file
    const pkg_data& pkg_info = open_spm(TMP_DIR + PName + ".spm");

    //Checking dependencies
    if (check_dependencies(pkg_info.dependencies,DATA_DIR)) 
    {
        std::cout << "dependencies are ok" << "\n";
        //installing  package with install_info command from the .spm file
        move_binaries(TMP_DIR,ROOT);
        std::cout << "package installed" << "\n";
        //cleaning 
        system(("rm -rf " + TMP_DIR).c_str());


    }
    else {
        std::cout << "dependencies are not ok" << "\n";
        //TODO:  DO SOMETHING HERE like with dependencies
    }
    //Returning 1 means the program ran successfully
    return 1;
}

//Creating a binary package from a .spm file
void create_binary (const std::string& PName)
{
    std::cout << "processing package " << PName << "\n"; 
    //Getting package data from .spm file
    const pkg_data& pkg_info = open_spm(PKG_DIR + PName + ".spm"); 
    bin_spm(PKG_DIR + PName + ".spm", WORK_DIR + "build/"+ PName + ".spm");
    //downloading package source into the work directory
    download_pkg(pkg_info.download_info, WORK_DIR);
    //making the package from source
    make_pkg(PName, pkg_info.build_info, WORK_DIR);
    //Creating the tar.gz package archive
    std::string cmd_archive = "(cd " + WORK_DIR + "build && tar -cvf " + std::filesystem::current_path().string() +"/"+ PName + ".tar.gz *)" ; // TODO fix these lines
    std::cout << cmd_archive << std::endl;
    system((cmd_archive).c_str());
    //cleaning build directory
    system(("rm -rf " + WORK_DIR + "build/*").c_str());
}