/*
Guys , this is the cccp , a communist package manager.
My function and variables names are awful , I know.
Good luck to everyone that will try to understand this shit.
Thank you for your help :)
*/

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <ios>
#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <vector>
#include <filesystem>

#include "../include/cccp.h"

// The filesystem root
std::string ROOT = "/";
// For real use it must be set to "/"

// main dir where all the files and packages will be stored and where we will work
std::string MAIN_DIR = ROOT + "var/cccp/";
// the dir where we will be building the packages and downoading the sources
//  TODO: proper dir names , but i dont have time to do it
std::string WORK_DIR = MAIN_DIR + "work/";
// the dir where the package file are storeds
std::string PKG_DIR = MAIN_DIR + "pkg/";
// the dir where the data is stored
std::string DATA_DIR = MAIN_DIR + "data/";
//where we store tests and logs
std::string LOG_DIR = MAIN_DIR + "log/";
// where the sources are stored for local packages
std::string SRC_DIR = MAIN_DIR + "src/";
// where the binaries are stored
std::string BIN_DIR = MAIN_DIR + "bin/";
// Dir where built binaries are stored after making or after uncompressing
std::string BUILD_DIR = WORK_DIR + "build/";
// Dir where the package sources are downloaded and built
std::string MAKE_DIR = WORK_DIR + "sources/";
//dir where specialscripts are stored
std::string SPECIAL_DIR = MAIN_DIR + "special/";

bool DEBUG = false; // set to true to see the debug messages
bool TESTING = false; // set to true to see the test messages

Soviet::LogConfig logConfig = { true, true };
Soviet::Log logger(logConfig);

// Main function
int main(int argc, char *argv[])
{
    // TODO: imporve this bad argument handling
    //verifying if the user has entered arguments
    if (argc < 2) {
        std::cout << "No arguments given. Terminating.\n";
        exit(1);
    }
    //preparing dirs
    prepare_dirs(); // create the directories if they dont exist
    for (int i = 1; i < argc; i++) {
        std::string option = argv[i];
        if (option.substr(0,1) == "-") {
            if (option == "--debug" || option == "-d")
            {
                DEBUG = true;
                std::cout << "Debug mode enabled.\n";
            }
            else if (option == "--test" || option == "-t")
            {
                TESTING = true;
                std::cout << "testing mode enabled.\n";
            }
            // add a option to change the ROOT 
            else if (option == "--root" || option == "-r")
            {
                if (i + 1 < argc)
                {
                    ROOT = argv[i + 1];
                    ROOT = ROOT + "/";
                    i++;
                    // TODO:Find a better way to do this
                    MAIN_DIR = ROOT + "var/cccp/";
                    DATA_DIR = MAIN_DIR + "data/";
                    LOG_DIR = MAIN_DIR + "log/";
                    BIN_DIR = MAIN_DIR + "bin/";
                    //preparing dirs
                    prepare_dirs(); // create the directories if they dont exist
                }
                else
                {
                    std::cout << "No root path given. Terminating.\n";
                    exit(1);
                }
            }
            else if (option == "--install")
            {
                install_package(argv[argc - 1]);
                i++;
            }
            else if (option == "--create")
            {
                create_binary(argv[argc - 1]);
                i++;
            }
            else if (option == "--binary")
            {
                install_binary(argv[argc - 1]);
                i++;
            }
            // the remove option , calling rm_pkg
            else if (option == "--remove")
            {
                std::string PPath = DATA_DIR + argv[argc - 1] + ".spm";
                logger.Print<Soviet::INFO>("Removing package %s\n", argv[argc - 1]);
                if (DEBUG) // use a macro instead...
                    logger.Print<Soviet::INFO>("Package info path is %s \n",PPath.c_str());
                if (!std::filesystem::exists(PPath))
                {
                    logger.Print<Soviet::ERROR>("Package file %s does not exist. Terminating.\n", argv[argc - 1]);
                    exit(1);
                }
                rm_pkg(DATA_DIR + argv[argc - 1] + ".spm", DATA_DIR,DEBUG);
                i++;
            }
            else if (option == "--check")
            {
                std::string PPath = DATA_DIR + argv[argc - 1] + ".spm";
                // checking if the package file exists 
                if (!std::filesystem::exists(PPath))
                {
                    logger.Print<Soviet::ERROR>("Package %s does not exist. Terminating.\n", argv[argc - 1]);
                    exit(1);
                }
                if (check_pkg(PPath, DATA_DIR, DEBUG))
                    logger.Print<Soviet::INFO>("Package %s is installed. Everything is okay!\n", argv[argc - 1]);
                else
                    logger.Print<Soviet::INFO>("Package %s is not installed. Or something went wrong.\n", argv[argc - 1]);
                i++;
            }
            else if (option == "--dev")
            {
                //This option is for developers only
                // dont use it unless you know what you are doing
                logger.Print<Soviet::WARNING>("Developer mode enabled.\n"); 
                init_data(DATA_DIR + "packages.json");
            }
            // Check if debug is enabled
            else
            {
                logger.Print<Soviet::ERROR>("Invalid option. Terminating.\n");
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
    }
    

    
    //Returning 0 means the program ran successfully
    // 
    return 0;
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

    // Checking dependencies
    if (check_dependencies(pkg_info.dependencies, DATA_DIR))
    {
        std::cout << "Dependency check passed"<< "\n";
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
        // Storing package data
        // Adding the locations to the package files , and the packages files to DATA_DIR
        store_spm(PPath, BUILD_DIR, DATA_DIR + PName + ".spm");
    }
    else
    {
        std::cout << "Dependency check failed"<< "\n";
        // TODO: ADD THE DEPENDENCIES STUFF HERE
        exit(1);
    }

    // Moving built binaries to their install location on the system
    move_binaries(BUILD_DIR, ROOT);
    std::string special_cmd = "SPECIAL_DIR=" + SPECIAL_DIR + "; " + pkg_info.special_info;
    system(special_cmd.c_str());
}

// this function installs a binary package
int install_binary(const std::string &PName)
{
    
    // Uncompressing the binary package into the temorary dir
    std::string cmd_uncompress = "tar -xvf " + BIN_DIR + PName + ".tar.gz -C " + BUILD_DIR;
    // Debug log of the command
    if (DEBUG)
        std::cout << cmd_uncompress << "\n";
    // executing the command
    system((cmd_uncompress).c_str());
    // Reading package data from .spm file
    const pkg_data &pkg_info = open_spm(BUILD_DIR + PName + ".spm");
    // add the spm to the datas
    system(("mv " + BUILD_DIR + PName + ".spm " + DATA_DIR).c_str());
    // Checking dependencies
    if (check_dependencies(pkg_info.dependencies, DATA_DIR))
    {
        std::cout << "Dependency check passed"<< "\n"; 
        
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
    std::string special_cmd = "SPECIAL_DIR=" + SPECIAL_DIR + "; " + pkg_info.special_info;
    system(special_cmd.c_str());

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
        if (DEBUG)
            std::cout << cmd_source << "\n";
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
    std::cout << cmd_archive << "\n";
    system(cmd_archive.c_str());
    // cleaning build directory
    system(("rm -rf " + BUILD_DIR + "*").c_str());
}
void prepare_dirs()
{
    // Creating the directories if they don't exist
    mkdir(LOG_DIR.c_str(), 0777);
    mkdir(SPECIAL_DIR.c_str(), 0777);
    mkdir(PKG_DIR.c_str(), 0777);
    mkdir(SRC_DIR.c_str(), 0777);
    mkdir(BIN_DIR.c_str(), 0777);
    mkdir(BUILD_DIR.c_str(), 0777);
    mkdir(WORK_DIR.c_str(), 0777);
    mkdir(MAKE_DIR.c_str(), 0777);
    mkdir(DATA_DIR.c_str(), 0777);
    if (DEBUG)
        std::cout << "Directories created successfully, the problem is elsewhere."<< "\n";
}