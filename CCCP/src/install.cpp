#include <fstream>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <vector>
#include <filesystem>


#include "../lib/pkg_data.h"

//including header file
#include "../include/pkg.h"
#include "../include/spm.h"
#include "../include/data.h"
#include "../include/deps.h"
#include "../include/move.h"





/*
In this file we are installing source or binary packages.
Source packages are archive files containing the source code of the package ,the post install script and an .spm file for the install commands.
Binary packages are archive files containing the compiled binary files of the package , the post install script and an .spm file for the install commands.
*/


// parsing data and installing package
void install_source(const std::string &PName,const std::string &PKG_DIR,const std::string &DATA_FILE,const std::string &DATA_DIR,const std::string &MAKE_DIR,const std::string &BUILD_DIR,const std::string &LOG_DIR,const std::string &ROOT)
{

    //uncompressing <PName>.src.spm.tar.gz in PKG_DIR
    std::string cmd_uncompress = "( cd " + PKG_DIR + " && tar -xf " + PName + ".src.spm.tar.gz -C " + MAKE_DIR + " )"; 
    
    // Reading spm file in MAKE DIR
    std::string spm_path = MAKE_DIR + PName + ".spm";
    const pkg_data &pkg_info = open_spm(spm_path);
    // Checking dependencies
    //This dependencies if;else system is deprecated and will be removed in the future
    if (check_dependencies(pkg_info.dependencies, DATA_DIR))
    {
        std::cout << "Dependency check passed"<< "\n";     
    }
    else
    {
        std::cout << "Dependency check failed"<< "\n";
        // TODO: ADD THE DEPENDENCIES STUFF HERE
        exit(1);
    }
    make_pkg(pkg_info, MAKE_DIR, BUILD_DIR,LOG_DIR);
    std::cout << "☭ Package built"<< "\n";

    // Moving built binaries to their install location on the system
    move_binaries(files, ROOT);
    //executing post installation scripts
    if (!pkg_info.special_info.empty()) 
    {
        system((SPECIAL_DIR + pkg_info.special_info).c_str());  
    }
    else 
    {
        if (DEBUG) std::cout << "No post installation scripts found" << "\n";
    }
    // Storing package data
    // Adding the locations to the package files , and the packages files to DATA_DIR
    store_spm(PPath, BUILD_DIR, DATA_DIR + PName + ".spm");
    //adding the package to the data file
    add_pkg_data(DATA_FILE,pkg_info.name,pkg_info.version);

}

// this function installs a binary package
int install_binary(const std::string &PName)
{
    //check if the package is already installed
    if (std::filesystem::exists(DATA_DIR + PName + ".spm"))
    {
        logger.Print<Soviet::INFO>("Package %s is already installed. Reinstalling.\n", PName.c_str());
        // removing the package
        rm_pkg(PName, DATA_DIR,DATA_FILE,DEBUG);
    }
    // Uncompressing the binary package into the temorary dir
    std::string cmd_uncompress = "tar -xf " + BIN_DIR + PName + ".tar.gz -C " + BUILD_DIR;
    // the name of the spm package file (its good )
    std::string SName = PName + ".spm";
    // Debug log of the command
    if (DEBUG)
        std::cout << cmd_uncompress << "\n";
    // executing the command
    system((cmd_uncompress).c_str());
    // Reading package data from .spm file
    const pkg_data &pkg_info = open_spm(BUILD_DIR + SName);
    
    // Checking dependencies
    if (check_dependencies(pkg_info.dependencies, DATA_DIR))
    {
        logger.Print<Soviet::INFO>("Dependency check passed\n"); 
        
    }
    else
    {
        std::cout << "Dependency check failed"<< "\n";
        // TODO:  DO SOMETHING HERE like with dependencies
    }
    
    // installing  package with install_info command from the .spm file
    move_binaries(BUILD_DIR, ROOT);
    std::cout << "☭ Package Installed, Comrade"<< "\n";

    //executing post installation scripts
    if ( not pkg_info.special_info.empty()) 
    {
        system((BUILD_DIR + pkg_info.special_info).c_str());  
    }
    else 
    {
        if (DEBUG) std::cout << "No post installation scripts found" << "\n";
    }

    // adding the package to the data file
    // add the spm to the datas
    std::filesystem::rename(BUILD_DIR + SName , DATA_DIR + SName);
    add_pkg_data(DATA_FILE,pkg_info.name,pkg_info.version);
    // Cleaning everything
    system(("rm -rf " + BUILD_DIR + "*").c_str());
    // Returning 1 means the program ran successfully
    return 1;
}


