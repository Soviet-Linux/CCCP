#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

#include "cccp.h"

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
    if (argc < 2) 
    {
        std::cout << "No arguments given! Terminating...\n";
        return 1;
    }
    bool DEBUG = false;
    bool TESTING = false;
    // A way to store the action arguments 
    // This isnt optimal but i dont know how to do it better
    actionList action ;
    // The packages to be installed or removed
    std::vector<std::string> parameters;

    for (int i = 1;i < argc;i++)
    {
        std::string option = argv[i];
        if (option.substr(0,1) == "-")
        {
            if (option.substr(0,2) == "--")
            {

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
                            // Synchronize mirrors
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
                        case 'g':
                            // Get packages
                            action = GET;
                            break;
                        case 'p' :
                            //install local
                            action = INSTALL_LOCAL;
                            break;
                        case 'b' :
                            //build
                            action = CREATE;
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
    if (DEBUG) std::cout << "calling libspm with parsed args" << std::endl;
    cccp(action,parameters,DEBUG,TESTING);
    if (DEBUG) std::cout << "Libspm call succesfull" << std::endl;
    return 0;
}
