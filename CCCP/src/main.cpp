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
const std::string ROOT = "/";
//For real use it must be set to "/"

//the dir where we will be building the packages and downoading the sources
const std::string WORK_DIR = ROOT + "/var/cccp/work/";
//the dir where the package file are storeds
const std::string PKG_DIR = ROOT + "/var/cccp/pkg/";
//the dir where the data is stored
const std::string DATA_DIR = ROOT + "/var/cccp/data/";
//where the sources are stored for local packages
const std::string SRC_DIR = ROOT + "/var/cccp/src/";
//where the binaries are stored
const std::string BIN_DIR = ROOT + "/var/cccp/bin/";

bool DEBUG = false; //set to true to see the debug messages

//Main function
int main (int argc, char *argv[]) 
{
    //verifying if the user has entered arguments
    if (argc < 2) {
        std::cout << "No arguments given. Terminating.\n";
        exit(1);
    }

    std::string option = argv[1];
    if (option == "--debug" || option == "-d")
    {
        DEBUG = true;
        std::cout << "Debug mode enabled.\n";
        option = argv[2];
    }
    //parsing argument 
    // TODO: use a switch statement here
    std::cout << "option : " << option << std::endl;
    if (option.substr(0,2) == "--") {
        if (option == "--install") {
            install_package(argv[argc-1]);
        }
        else if (option == "--create") {

            create_binary(argv[argc-1]);
        }
        else if (option == "--binary") {
            install_binary(argv[argc-1]);
        }
        else if (option == "--test")
        {
            std::cout << "Testing\n";
        }
        //Check if debug is enabled
        else
        {
            std::cout << "Invalid option. Terminating.\n";
            exit(1);
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
    //initialising package path
    std::string PPath = PKG_DIR + PName + ".spm";
    //debug message
    if (DEBUG) {
        std::cout << "PPath : " << PPath << "\n";
    }
    //Getting the package data from the spm file
    const pkg_data& pkg_info = open_spm(PPath); 
    //Debug message
    if (DEBUG) std::cout << pkg_info.name << " " << pkg_info.type << " " << pkg_info.version << "\n"; 

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
    system(("mkdir " + TMP_DIR).c_str());
    //Uncompressing the binary package into the temorary dir
    std::string cmd_uncompress = "tar -xvf " + BIN_DIR + PName + ".tar.gz -C " + TMP_DIR;
    //Debug log of the command
    if (DEBUG) std::cout << cmd_uncompress << "\n";
    //executing the command
    system((cmd_uncompress).c_str());
    //Reading package data from .spm file
    const pkg_data& pkg_info = open_spm(TMP_DIR + PName + ".spm");
    //add the spm to the datas
    // TODO: add a real database in db.cpp
    std::string cmd_clean = "mv -f " + TMP_DIR + PName + ".spm " + DATA_DIR;
    system(cmd_clean.c_str());
    //Checking dependencies
    if (check_dependencies(pkg_info.dependencies,DATA_DIR)) 
    {
        std::cout << "dependencies are ok" << "\n";
        //installing  package with install_info command from the .spm file
        move_binaries(TMP_DIR ,ROOT);
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
    if (pkg_info.type == "src")
    {
        //downloading package source into the work directory
        download_pkg(pkg_info.download_info, WORK_DIR);
    }
    else if (pkg_info.type == "local") {
        std::string cmd_source = "tar -xf " + SRC_DIR + PName + "*" + " -C " + WORK_DIR + "sources/";
        std::cout << cmd_source << "\n";
        system(cmd_source.c_str());
        
    }
   

    //making the package from source
    make_pkg(PName, pkg_info.build_info, WORK_DIR);
    std::cout << "package built" << "\n";
    //Creating the tar.gz package archive
    std::string cmd_archive = "( cd " + WORK_DIR + "build/ && tar -cvf " + BIN_DIR + PName + ".tar.gz * )";
    std::cout << cmd_archive << "\n";
    system(cmd_archive.c_str());
    //cleaning build directory
    system(("rm -rf " + WORK_DIR + "build/*").c_str());
}