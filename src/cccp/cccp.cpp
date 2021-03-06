#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

#include "cccp.h"

//
// This needs to be the same than in libspm/main.cpp or it will explode 
enum actionList {INSTALL_LOCAL,INSTALL_FROM_REPO,CHECK,LIST,REMOVE,CREATE,HELP,UPDATE,CLEAN,SYNC};

// Main function
int main(int argc, char *argv[])
{
    // checking if cccp is run as root
    if (getuid()) 
    {
        std::cout << "You need to be root to perform the operation ! " << std::endl;
        return 1;
    }
    //verifying if the user has entered arguments
    bool DEBUG = false;
    bool TESTING = false;
    // A way to store the action arguments 
    // This isnt optimal but i dont know how to do it better
    actionList action ;
    // The packages to be installed or removed
    std::vector<std::string> parameters;
    if (argc < 2)
    {
        cccp(6,parameters,DEBUG,TESTING);
    }
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
                    DEBUG = true;
                }
                else if (longOption == "testing") { 
                    TESTING = true;
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
                            std::cout << "Synchronizing the repositories package files from fontend\n";    
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
                            // Debug mode
                            DEBUG = true;
                            break;
                        case 't' :
                            // Testing mode
                            TESTING = true;
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
    cccp(int(action),parameters,DEBUG,TESTING);
    return 0;
}
