#include <fstream>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <vector>
#include <filesystem>




//including header file
#include "../include/make.h"
#include "../include/pkg.h"





// This function will check if all dependencies of a package are installed
int check_dependencies (const std::vector<std::string>& dependencies, const std::string& DATA_DIR) 
{
    //TODO : check if the dependencies are installed
    //
    //Print the dependencies
    if (dependencies[0] == "base")
    {
        std::cout << "this packages requires the base system , no dependencies checking.\n";
        return 1;
    }
    else {
        for (int i = 0; i < dependencies.size(); i++)
        {
            // TODO: implement vesrion checking
            if (std::filesystem::exists(DATA_DIR + dependencies[i] + ".spm"))
            {
                std::cout << "dependency " << dependencies[i] << " is installed.\n";
            }
            else {
                std::cout << "dependency " << dependencies[i] << " is not installed.\n";
                //install automatically the needed dependency
                //To do this run install_package
                install_package(dependencies[i]);
            }
        }
    }
   
    return 1;
}
// parsing data and installing package
void install_package(const std::string &PName)
{
    logger.Print<Soviet::INFO>("Processing package %s\n", PName.c_str());
    // initialising package path
    const std::string PPath = PKG_DIR + PName + ".spm";
    // checking if the package file exists 
    if (!std::filesystem::exists(PPath))
    {
        logger.Print<Soviet::ERROR>("Package %s does not exist. Terminating.\n", PName.c_str());
        exit(1);
    }
    //check if the package is already installed
    if (std::filesystem::exists(DATA_DIR + PName + ".spm"))
    {
        logger.Print<Soviet::INFO>("Package %s is already installed. Reinstalling.\n", PName.c_str());
        // removing the package
        rm_pkg(PName, DATA_DIR,DATA_FILE,DEBUG);
    }
    // debug message
    if (DEBUG)
    {
        logger.Print<Soviet::INFO>("PPath: %s\n", PPath.c_str());
    }
    // Getting the package data from the spm file
    const pkg_data &pkg_info = open_spm(PPath);
    // Debug message
    if (DEBUG) // mAAccccrrrrOO
        std::cout << pkg_info.name << " " << pkg_info.type << " " << pkg_info.version << "\n";

    // SOMEONE REPLACE ALL std::cout WITH THE NEW LOGGER PLZ
    // OK i'll do it later
    // TODO: do that 

    // Checking dependencies
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

    if (pkg_info.type == "src")
    {
        // downloading package source into the work directory
        const std::string &download_cmd = "( cd " + MAKE_DIR + " && " + pkg_info.download_info + " )";

        std::cout << download_cmd << "\n";
        system(download_cmd.c_str());
    }
    else if (pkg_info.type == "local")
    {
        std::string cmd_archive = "tar -xf " + SRC_DIR + PName + "*tar*" + " -C " + MAKE_DIR;
        std::cout << cmd_archive << "\n";
        system(cmd_archive.c_str());
    }  
    // making the package from source
    make_pkg(pkg_info, MAKE_DIR, BUILD_DIR,LOG_DIR,TESTING);
    std::cout << "☭ Package built"<< "\n";

    

    

    // Moving built binaries to their install location on the system
    move_binaries(BUILD_DIR, ROOT);
    //executing post installation scripts
    if ( not pkg_info.special_info.empty()) 
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

// Creating a binary package from a .spm file
void create_binary(const std::string &PName)
{
    // PKG file location
    std::string PPath = PKG_DIR + PName + ".spm";
    // checking if the package file exists 
    if (!std::filesystem::exists(PPath))
    {
        logger.Print<Soviet::ERROR>("Package %s does not exist. Terminating.\n", PName.c_str());
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

