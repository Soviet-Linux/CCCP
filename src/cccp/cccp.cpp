#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

#include "../../include/shared.h"

#define RELEASE 0.1

configs FConf;

// Main function
int main(int argc, char *argv[])
{

    // configurring the default dir structure
    /*
    Here is a more detailed look of the default directory structure 
    / --> ROOT
    ├──temp --> TMP_DIR
    ├──etc
    │   └── cccp.conf
    └── var
        └── cccp --> MAIN_DIR
            ├── data --> DATA_DIR
            |   └── packages.json
            ├── spm --> SPM_DIR
            ├── log --> LOG_DIR
            └── work --> WORK_DIR
                ├── build --> BUILD_DIR (also called $BUILD_ROOT)
                └── make --> MAKE_DIR

    */
    FConf.ROOT = "/";
    FConf.MAIN_DIR = FConf.ROOT + "var/cccp";
    FConf.DATA_DIR = FConf.MAIN_DIR + "/data";
    FConf.SPM_DIR = FConf.MAIN_DIR + "/spm";
    FConf.LOG_DIR = FConf.MAIN_DIR + "/log";
    FConf.WORK_DIR = FConf.MAIN_DIR + "/work";
    FConf.BUILD_DIR = FConf.WORK_DIR + "/build";
    FConf.MAKE_DIR = FConf.WORK_DIR + "/make";
    FConf.TMP_DIR = FConf.ROOT + "tmp/spm.tmp.d";

    FConf.CONFIG_FILE = "/etc/cccp.conf";

    FConf.ALL_FILE = FConf.DATA_DIR + "/all.json";
    FConf.INSTALLED_FILE = FConf.DATA_DIR + "/installed.json";
    // setting the main config variables
    FConf.DEBUG = false;
    FConf.TESTING = false;
    FConf.QUIET = true;
    FConf.OVERWRITE = false;


    // checking if cccp is run as root
    if (getuid()) 
    {
        std::cout << "You need to be root to perform the operation ! " << std::endl;
        return 1;
    }
    //verifying if the user has entered arguments
    if (argc < 2) 
    {
        std::cout << "No arguments given! Terminating...\n";
        return 1;
    }

    // A way to store the action arguments 
    // This isnt optimal but i dont know how to do it better
    actionList action = HELP;
    // The packages to be installed or removed
    std::vector<std::string> parameters;

    for (int i = 1;i < argc;i++)
    {
        std::string option = argv[i];
        if (option.substr(0,1) == "-")
        {
            if (option.substr(0,2) == "--")
            {
                std::string longOption = option.substr(2,option.length());
                // if else s
                if (longOption == "help") { 
                    action = HELP;
                }
                else if (longOption == "sync") { 
                    action = SYNC;
                }
                else if (longOption == "clean") { 
                    action = CLEAN;
                }
                else if (longOption == "update") { 
                    action = UPDATE;
                }
                else if (longOption == "version") { 
                    std::cout << "CCCP C++ front-end version " << RELEASE << std::endl;
                    std::cout << "LIBSPM C++ version " << version() << std::endl;
                    return 0;
                }
                else if (longOption == "debug") { 
                    FConf.DEBUG = true;
                }
                else if (longOption == "testing") { 
                    FConf.TESTING = true;
                }
                else if (longOption == "check") { 
                    action = CHECK;
                }
                else if (longOption == "list") { 
                    action = LIST;
                }
                else if (longOption == "remove") { 
                    action = REMOVE;
                }
                else if (longOption == "binary") { 
                    action = CREATE;
                }
                else if (longOption == "package") { 
                    action = INSTALL_LOCAL;
                }
                else if (longOption == "install") {
                     action = INSTALL_FROM_REPO;
                }
                else if (longOption == "force" or longOption == "overwrite")
                {
                    FConf.OVERWRITE = true;
                }
                else if (longOption == "quiet")
                {
                    FConf.QUIET = true;
                }
                else if (longOption == "verbose")
                {
                    FConf.QUIET = false;
                }
                else { std::cout << "Unknown option " << option << "! Terminating...\n"; return 1;}


            }
            else  
            {
                for (int i = 1;i < option.size();i++)
                {
                    switch (option[i]) 
                    {
                        case 'h' :
                            // print a real help message ( TODO: improve the message)
                            action = HELP;
                            break;
                        case 's' :
                            //debug
                            // Synchronize mirrors
                            action = SYNC;
                            break;

                        case 'i' :
                            // Install packages
                            action = INSTALL_FROM_REPO;
                            break;
                        case 'r' :
                            // Remove packages
                            action = REMOVE;
                            break;
                        case 'u' :
                            // Update packages
                            // We are very far from an update system so i wont touch this in a while
                            break;
                        case 'c' :
                            // Check packages
                            std::cout << "Checking packages...\n";
                            action = CHECK;
                            break;
                        case 'C' :
                            // Clean packages
                            std::cout << "Cleaning work dirs...\n";
                            action = CLEAN;
                            break;
                        case 'l' :
                            // List packages
                            action = LIST;
                            break;
                        case 'd' :
                            //FConf.DEBUG mode
                           FConf.DEBUG = true;
                            switch (option[i+1]) 
                            {
                                case '1' :
                                    //FConf.DEBUG mode 1
                                   FConf.DEBUG = 1;
                                    break;
                                case '2' :  
                                    //FConf.DEBUG mode 2
                                   FConf.DEBUG = 2;
                                    break;
                                case '3' :
                                    //FConf.DEBUG mode 3
                                   FConf.DEBUG = 3;
                                    break;
                                default :
                                   FConf.DEBUG = 1;     
                            }
                            //This message is ugly but i cant change it because i cant access the soviet::msg fucntion from here
                            std::cout << "\033[1m\033[32m" << " DEBUG: " << "\033[0m" << "\033[32m" <<"Enabling level " << FConf.DEBUG <<" debug mode" <<  "\033[0m" << std::endl;
                            // incrementing the i to skip the next character
                            i++;

                            break;
                        case 't' :
                            //FConf.TESTING mode
                           FConf.TESTING = true;
                            break;
                        case 'p' :
                            //install local
                            action = INSTALL_LOCAL;
                            break;
                        case 'b' :
                            //build
                            action = CREATE;
                            break;
                        case 'v' :
                            std::cout << "CCCP C++ front-end version " << RELEASE << std::endl;
                            std::cout << "LIBSPM C++ version " << version() << std::endl;
                            return 0;
                        case 'f' :
                            //overwrite
                            FConf.OVERWRITE = true;
                            break;
                        case 'q' :
                            //quiet
                            FConf.QUIET = true;
                            break;  
                        case 'V' :
                            //verbose
                            FConf.QUIET = false;
                            break;
                        default:
                            // Unknown option
                            std::cout << "Unknown option! Terminating...\n";
                            break;

                
                    }
                }
            }
        }
        else
        {
            if (i > 1)
            {
                parameters.push_back(option);
                
            }
            else
            { 
                action = HELP;
                break;
            }
        }
    }

    cccp(int(action),parameters,FConf);
    return 0;
}
