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
