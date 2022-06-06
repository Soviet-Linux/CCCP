/*
Guys , this is the cccp , a communist package manager.
My function and variables names are awful , I know.
Good luck to everyone that will try to understand this shit.
Thank you for your help :)
*/


#include <cstdlib>
#include <stdexcept>
#include <string>
#include <vector>

#include "../../include/cccp.hpp"


// The filesystem root
std::string soviet::ROOT = "/";
// For real use it must be set to "/"

// main dir where all the files and packages will be stored and where we will work
std::string soviet::MAIN_DIR = soviet::ROOT + "var/cccp/";
// the dir where we will be building the packages and downoading the sources
//  TODO: proper dir names , but i dont have time to do it
std::string soviet::WORK_DIR = soviet::MAIN_DIR + "work/";
// the dir where the data is stored
std::string soviet::DATA_DIR = soviet::MAIN_DIR + "data/";
// where spm files are stored
// TODO: urgent , impement that before release
std::string soviet::SPM_DIR = soviet::MAIN_DIR + "spm/";
//where we store tests and logs
std::string soviet::LOG_DIR = soviet::MAIN_DIR + "log/";
// Dir where built binaries are stored after making or after uncompressing
std::string soviet::BUILD_DIR = soviet::WORK_DIR + "build/";
// Dir where the package sources are downloaded and built
std::string soviet::MAKE_DIR = soviet::WORK_DIR + "make/";
//The file where a lot of data are stored
std::string soviet::INSTALLED_FILE = soviet::DATA_DIR + "installed.json";
std::string soviet::ALL_FILE = soviet::DATA_DIR + "all.json";
// configuraton file
std::string soviet::CONFIG_FILE = soviet::ROOT + "cccp.conf";
//package repos
std::vector<std::string> soviet::REPOS;
/*
Here is a more detailed look of the default directory structure 
/ --> ROOT
├──etc
│   └── cccp.conf
└── var
    └── cccp --> MAIN_DIR
        ├── data --> DATA_DIR
        │   └── packages.json
        ├── spm --> SPM_DIR
        ├── log --> LOG_DIR
        └── work --> WORK_DIR
            ├── build --> BUILD_DIR (also called $BUILD_ROOT)
            └── make --> MAKE_DIR

*/
bool soviet::DEBUG = false;
bool soviet::TESTING = false;

// Main function
int main(int argc, char *argv[])
{

    // Prepare the cccp
    soviet::init();
    //verifying if the user has entered arguments
    if (argc < 2) {
        std::cout << "No arguments given! Terminating...\n";
        exit(1);
    }
    
    // A way to store the action arguments 
    // This isnt optimal but i dont know how to do it better
    std::vector<soviet::action> action ;
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
                            soviet::help();
                            break;
                        case 's' :
                            // Synchronize mirrors
                            break;

                        case 'i' :
                            // Install packages
                            action.push_back(soviet::INSTALL_FROM_REPO);
                            break;
                        case 'r' :
                            // Remove packages
                            action.push_back(soviet::REMOVE);
                            break;
                        case 'u' :
                            // Update packages
                            // We are very far from an update system so i wont touch this in a while
                            break;
                        case 'c' :
                            // Check packages
                            std::cout << "Checking packages...\n";
                            action.push_back(soviet::CHECK);
                            break;
                        case 'l' :
                            // List packages
                            action.push_back(soviet::LIST);
                            break;
                        case 'd' :
                            // Debug mode
                            soviet::DEBUG = true;
                            break;
                        case 't' :
                            // Testing mode
                            soviet::TESTING = true;
                            break;
                        case 'g':
                            // Get packages
                            action.push_back(soviet::GET);
                            break;
                        case 'p' :
                            //install local
                            action.push_back(soviet::INSTALL_LOCAL);
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
                soviet::help();
                std::cout << "No action argument given! Terminating...\n"; 
                exit(1);
            }
        }
    }
    // For all actions
    for (int i = 0;i < action.size();i++)
    {  
        switch (action[i])
        {   
            case soviet::INSTALL_LOCAL :
                // Install packages 
                // Verify if parameters are given
                if (parameters.empty())
                {      
                    std::cout << "No packages specified! Terminating...\n";
                }         
                for (int i = 0;i < parameters.size();i++)
                {
                    std::cout << "Installing " << parameters[i] << "\n";

                    soviet::package pkg;
                    pkg.packagePath = parameters[i];
                    
                    if (pkg.packagePath.length() < 15) 
                    {
                        std::cout << "Package path is too short; maybe it's not a package? Terminating...\n";
                        exit(1);
                    }
                    std::string extension = pkg.packagePath.substr(pkg.packagePath.find_first_of("."),pkg.packagePath.length());
                    pkg.name = pkg.packagePath.substr(0,pkg.packagePath.find_first_of("."));
                    //initialize variables
                    
                    pkg.dataSpmPath = soviet::SPM_DIR + pkg.name + ".spm";
                    if (extension == ".src.spm.tar.gz")
                    {
                        pkg.type = "src";
                    }
                    else if (extension == ".bin.spm.tar.gz") 
                    {
                        pkg.type = "bin";
                    }
                    else 
                    {
                        std::cout << "The file is not a package. Terminating...\n";
                        exit(1);
                    }
                    if (soviet::DEBUG) std::cout << "launching installation with " << pkg.packagePath << "\n";
                    pkg.installFile();
                }
                break;
            case soviet::REMOVE :
                for (int i = 0;i < parameters.size();i++)
                {
                    std::cout << "Removing " << parameters[i] << "\n";
                    soviet::package pkg;
                    pkg.name = parameters[i];
                    pkg.dataSpmPath = soviet::SPM_DIR + pkg.name + ".spm";
                    pkg.uninstall();
                    
                }
                break;
            case soviet::LIST :
                std::cout << "Listing packages\n";
                soviet::listPkgs();
            case soviet::CHECK :
                for (int i = 0;i < parameters.size();i++)
                {
                    std::cout << "Checking " << parameters[i] << "\n";
                    soviet::package pkg;
                    pkg.name = parameters[i];
                    if (pkg.check())
                    {
                        std::cout << "Package " << parameters[i] << " is installed and good!\n";
                    }
                    else 
                    {
                        std::cout << "Package " << parameters[i] << " is not installed or corrupted!\n";
                    }
                }
                break;
            case soviet::INSTALL_FROM_REPO :
                for (int i = 0;i < parameters.size();i++)
                {
                    std::cout << "Getting " << parameters[i] << "\n";
                    soviet::package pkg;
                    pkg.name = parameters[i];
                    pkg.get();
                }
                break;
            default :
                std::cout << "Action error! Terminating...\n";
                exit(1);
        }
    }

    //Returning 0 means the program ran successfully
    // 
    return 0;
}

/*
Usage is :
cccp -Si package1 package2 package3
cccp -i package1 package2 package3
cccp -h
cccp --install package1 package2 package3
cccp --sync 
With the 'p' option , the package mustbe src.spm.tar.gz
*/
/*
package structure is :
    src -> $NAME.src.spm.tar.gz
            ├──$NAME.spm
            └──$NAME-$VERSION
                └──[sources]
    bin -> $NAME.bin.spm.tar.gz
            ├──$NAME.spm
            └──$NAME-$VERSION
                └──[sources]
		
*/

