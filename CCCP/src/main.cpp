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
#include "../lib/log.hpp"

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
//dir where special scripts are stored
std::string SPECIAL_DIR = MAIN_DIR + "special/";
//The file where a lot of data are stored
std::string DATA_FILE = DATA_DIR + "packages.json";
/*
Here is a more detailed look of the default directory structure 
/ --> ROOT
└── var
    └── cccp --> MAIN_DIR
        ├── bin --> BIN_DIR
        ├── data --> DATA_DIR
        ├── log --> LOG_DIR
        ├── pkg --> PKG_DIR
        ├── special --> SPECIAL_DIR
        ├── src --> SRC_DIR
        └── work --> WORK_DIR
            ├── build --> BUILD_DIR
            └── sources --> MAKE_DIR

*/

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
    if (not std::filesystem::exists(DATA_FILE)) init_data(DATA_FILE); // create the data file  

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
                std::string PName = argv[argc - 1] ;
                logger.Print<Soviet::INFO>("Removing package %s\n", argv[argc - 1]);
                if (!std::filesystem::exists(DATA_DIR + PName + ".spm"))
                {
                    logger.Print<Soviet::ERROR>("Package file %s does not exist. Terminating.\n", argv[argc - 1]);
                    exit(1);
                }
                rm_pkg(PName, DATA_DIR,DATA_FILE,DEBUG);
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
            std::cout << "To remove a package : \n";
            std ::cout << "Usage: cccp --remove <package_name>\n\n";
            std::cout << "To check if a package is installed : \n";
            std ::cout << "Usage: cccp --check <package_name>\n\n";
            std::cout << "To initiate the data directory (Do this if you think your system is fucked because it will destroy eveything): \n";
            std ::cout << "Usage: cccp --initiate\n\n";
            exit(1);
        } 
    }
    

    
    //Returning 0 means the program ran successfully
    // 
    return 0;
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