/*
Guys , this is the cccp , a communist package manager.
My function and variables names are awful , I know.
Good luck to everyone that will try to understand this shit.
Thank you for your help :)
*/


#include <stdexcept>
#include <string>
#include <vector>

#include "../include/cccp.hpp"


// The filesystem root
std::string soviet::ROOT = "/";
// For real use it must be set to "/"

// main dir where all the files and packages will be stored and where we will work
std::string soviet::MAIN_DIR = soviet::ROOT + "var/cccp/";
// the dir where we will be building the packages and downoading the sources
//  TODO: proper dir names , but i dont have time to do it
std::string soviet::WORK_DIR = soviet::MAIN_DIR + "work/";
// the dir where the package file are storeds
std::string soviet::PKG_DIR = soviet::MAIN_DIR + "pkg/";
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
std::string soviet::DATA_FILE = soviet::DATA_DIR + "packages.json";
/*
Here is a more detailed look of the default directory structure 
/ --> ROOT
└── var
    └── cccp --> MAIN_DIR
        ├── data --> DATA_DIR
        ├── spm --> SPM_DIR
        ├── log --> LOG_DIR
        └── work --> WORK_DIR
            ├── build --> BUILD_DIR (also called $BUILD_ROOT)
            └── make --> MAKE_DIR

*/

// Main function
int main(int argc, char *argv[])
{
    //verifying if the user has entered arguments
    if (argc < 2) {
        throw std::runtime_error("No arguments given. Terminating.\n");
    }

    // A way to store the action arguments 
    // This isnt optimal but i dont know how to do it better
    soviet::action action;
    // The packages to be installed or removed
    std::vector<std::string> parameters;
    //Dir where the package is stored
    std::string packageDir;

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
                for (int i = 2;i < option.size();i++)
                {
                    switch (option[i]) 
                    {
                        case 'h' :
                            // print a real help message ( TODO: improve the message)
                            std::cout << "Help message\n";
                            break;
                        case 'S' :
                            // Synchronize mirrors
                            break;

                        case 'i' :
                            // Install packages
                            std::cout << "Installing packages\n";
                            action = soviet::INSTALL;
                            break;
                        case 'r' :
                            // Remove packages
                            std::cout << "Removing packages\n";
                            action = soviet::REMOVE;
                            break;
                        case 'u' :
                            // Update packages
                            // We are very far from an update system so i wont touch this in a while
                            break;
                        case 'c' :
                            // Check packages
                            std::cout << "Checking packages\n";
                            action = soviet::CHECK;
                            break;
                        case 'l' :
                            // List packages
                            std::cout << "Listing packages\n";
                            action = soviet::LIST;
                            break;
                        case 'p' :
                            // Specify package dir 
                
                    }
                }
            }
        }
        else
        {
            if (i == 1)
            {
                throw std::runtime_error("No action given. Terminating.\n");
            }
            else
            {
                parameters.push_back(option);
            }
        }
    }
    switch (action)
    {
        case soviet::INSTALL :
            for (int i = 0;i < parameters.size();i++)
            {
                std::cout << "Installing " << parameters[i] << "\n";
                soviet::package pkg;
                pkg.name = parameters[i];

                pkg.install();
            }
        default :
            throw std::runtime_error("Action error . Terminating.\n");
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
*/
