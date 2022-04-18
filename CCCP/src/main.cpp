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


//main dir where all the files and packages will be stored and where we will work
const std::string MAIN_DIR = ROOT + "var/cccp/";
//the dir where we will be building the packages and downoading the sources
// TODO: proper dir names , but i dont have time to do it
const std::string WORK_DIR = MAIN_DIR + "work/";
//the dir where the package file are storeds
const std::string PKG_DIR = MAIN_DIR + "pkg/";
//the dir where the data is stored
const std::string DATA_DIR = MAIN_DIR + "data/";
//where the sources are stored for local packages
const std::string SRC_DIR = MAIN_DIR + "src/";
//where the binaries are stored
const std::string BIN_DIR = MAIN_DIR + "bin/";
//Dir where built binaries are stored after making or after uncompressing
const std::string BUILD_DIR = WORK_DIR + "build/";
//Dir where the package sources are downloaded and built
const std::string MAKE_DIR = WORK_DIR + "sources/";

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
        // the remove option , calling rm_pkg
        else if (option == "--remove")
        {
            std::cout << "removing package " << argv[argc-1] << std::endl;
            if (DEBUG) std::cout << DATA_DIR + argv[argc-1] + ".spm" << std::endl;
            rm_pkg(DATA_DIR + argv[argc-1] + ".spm",DATA_DIR);           
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
    const std::string PPath = PKG_DIR + PName + ".spm";
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
            std::string cmd_archive = "tar -xf " + SRC_DIR + PName + "*" + " -C " + MAKE_DIR;
            std::cout << cmd_archive << "\n";
            system(cmd_archive.c_str());
        
        }
        
        //making the package from source
        make_pkg(PName, pkg_info.build_info, MAKE_DIR,BUILD_DIR);
        std::cout << "package built" << "\n";
        //Storing package data
        //Adding the locations to the package files , and the packages files to DATA_DIR
        store_spm(PPath,BUILD_DIR, DATA_DIR + PName + ".spm");
    }
    else {
        std::cout << "dependencies are not ok" << "\n";
        //TODO: ADD THE DEPENDENCIES STUFF HERE

    }
     
    //Moving built binaries to their install location on the system
    move_binaries(BUILD_DIR,ROOT);

}

//this function installs a binary package
int install_binary(const std::string& PName)
{
    //Uncompressing the binary package into the temorary dir
    std::string cmd_uncompress = "tar -xvf " + BIN_DIR + PName + ".tar.gz -C " + BUILD_DIR;
    //Debug log of the command
    if (DEBUG) std::cout << cmd_uncompress << "\n";
    //executing the command
    system((cmd_uncompress).c_str());
    //Reading package data from .spm file
    const pkg_data& pkg_info = open_spm(BUILD_DIR + PName + ".spm");
    //add the spm to the datas
    system(("mv " + BUILD_DIR + PName + ".spm " + DATA_DIR).c_str());
    //Checking dependencies
    if (check_dependencies(pkg_info.dependencies,DATA_DIR)) 
    {
        std::cout << "dependencies are ok" << "\n";
        //installing  package with install_info command from the .spm file
        move_binaries(BUILD_DIR ,ROOT);
        std::cout << "package installed" << "\n";
        //cleaning 
        system(("rm -rf " + BUILD_DIR + "*").c_str());


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
    //PKG file location
    std::string PPath = PKG_DIR + PName + ".spm";
    //temp spm file location ( I know , this is not very good , if you are very intelligent maybe you could change it :) 
    // TODO: change this to a better location
    std::string temp_path = "/tmp/temp.spm";

    std::cout << "processing package " << PName << "\n"; 
    //Getting package data from .spm file
    const pkg_data& pkg_info = open_spm(PPath); 
    if (pkg_info.type == "src")
    {
        //downloading package source into the work directory
        download_pkg(pkg_info.download_info, MAKE_DIR);
    }
    else if (pkg_info.type == "local") {
        //unpacking the sources archive
        std::string cmd_source = "tar -xf " + SRC_DIR + PName + "*" + " -C " + WORK_DIR + "sources/";
        if (DEBUG) std::cout << cmd_source << "\n";
        system(cmd_source.c_str());
        
    }
   

    //making the package from source
    make_pkg(PName, pkg_info.build_info, MAKE_DIR,BUILD_DIR);
    std::cout << "package built" << "\n";
    //adding locations and other thing to spm file
    bin_spm(PPath, temp_path);
    store_spm(temp_path,BUILD_DIR,BUILD_DIR + PName + ".spm");
    //Creating the tar.gz package archive
    std::string cmd_archive = "( cd " + BUILD_DIR + " && tar -cf " + BIN_DIR + PName + ".tar.gz * )";
    std::cout << cmd_archive << "\n";
    system(cmd_archive.c_str());
    //cleaning build directory
    system(("rm -rf " + BUILD_DIR + "*").c_str());
}