/*
Guys , this is the cccp , a communist package manager.
My function and variables names are awful , I know.
Good luck to everyone that will try to understand this shit.
Thank you for your help :)
*/
#include <map>
#define RELEASE 0.1

#include <cstdlib>
#include <stdexcept>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

#include "../../include/libspm.hpp"


// Declaration of the libspm main variables 
configs soviet::vars;






int cccp(int actionInt , std::vector<std::string> parameters, configs spmConfig)
{
    
    // pass the config to the libspm main variables
    soviet::vars = spmConfig;
    // Prepare the cccp
    soviet::init();
    //Declaring enum

    // casting the int parameter to an enum for the switch
    // this isnt optimal for c++ use but its better for compatibility with other languages like rust or python
    actionList action = (actionList)actionInt;

    // debug
    soviet::msg(soviet::level::DBG3, "Libspm called with action %s" , std::to_string(actionInt).c_str());
    soviet::msg(soviet::level::DBG3, "DEBUG: %s , TESTING: %s , OVERWRITE: %s , QUIET: %s" , std::to_string(soviet::vars.DEBUG).c_str(), std::to_string(soviet::vars.TESTING).c_str(), std::to_string(soviet::vars.OVERWRITE).c_str(), std::to_string(soviet::vars.QUIET).c_str());

    switch (action)
    {   
        case INSTALL_LOCAL :
            // Install packages 
            // Verify if parameters are given
            if (parameters.empty())
            {      
                soviet::msg(soviet::level::FATAL, "No packages specified! Terminating...");
            }         
            for (auto & param : parameters)
            {
                soviet::msg(soviet::level::INFO, "Installing %s", param.c_str());

                soviet::package pkg = soviet::parseFileName(param);
                
                soviet::msg(soviet::level::DBG1, "Launching installation with %s", pkg.packagePath.c_str());
                pkg.installArchive();
            }
            break;
        case REMOVE :
            for (auto & param : parameters)
            {
                soviet::msg(soviet::level::INFO, "Removing %s", param.c_str());
                soviet::package pkg;
                pkg.name = param;
                pkg.dataSpmPath = soviet::format("%s/%s.spm",soviet::vars.SPM_DIR.c_str(),pkg.name.c_str());
                pkg.uninstall();
                
            }
            break;
        case LIST :
            soviet::msg(soviet::level::INFO, "Listing packages");
            soviet::listPkgs();
        case CHECK :
            for (auto & param : parameters)
            {
                soviet::msg(soviet::level::INFO, "Checking %s", param.c_str());

                soviet::package pkg;
                pkg.name = param;
                pkg.dataSpmPath = soviet::vars.SPM_DIR + pkg.name + ".spm";
                // debug
                soviet::msg(soviet::level::DBG1, "Launching check for %s with %s", pkg.name.c_str(), pkg.dataSpmPath.c_str());
                if (pkg.check())
                {
                    std::cout << "Package " << param << " is installed and good!\n";

                }
                else 
                {
                    std::cout << "Package " << param << " is not installed or corrupted!\n";
                }
            }
            break;
        case INSTALL_FROM_REPO :
            for (auto & param : parameters)
            {
                soviet::msg(soviet::level::DBG1, "Getting %s", param.c_str());
                soviet::package pkg;
                pkg.name = param;
                pkg.dataSpmPath = soviet::format("%s/%s.spm", soviet::vars.SPM_DIR.c_str(), pkg.name.c_str());
                mkdir(soviet::vars.TMP_DIR.c_str(),0777);
                pkg.get();
                pkg.installArchive();
                std::filesystem::remove_all(soviet::vars.TMP_DIR);
            }
            break;
        case CREATE:
            if (parameters.empty())
            {      
                soviet::msg(soviet::level::FATAL, "No packages specified! Terminating...");
            }         
            for (auto & param : parameters)
            {

                soviet::msg(soviet::level::INFO, "Creating binary package from %s", param.c_str());

                soviet::package pkg = soviet::parseFileName(param);

                soviet::msg(soviet::level::DBG1, "Launching creation with %s", pkg.packagePath.c_str());
                
                pkg.createBinary(soviet::format("%s/%s.bin.spm.tar.gz",std::filesystem::current_path().c_str(),pkg.name.c_str()));
            }
            break;
        case HELP:
            soviet::help();
            break;
        case CLEAN:
            soviet::clean();
            break;
        case SYNC:
            soviet::msg(soviet::level::INFO, "Syncing package files");
            soviet::sync();
            break;
        case PRINT :
            // here we are printing the content of a package data file
            // first we need to loop over the parameters and find the package we want to print
            for (auto & param : parameters)
            {
                soviet::msg(soviet::level::INFO, "Printing %s", param.c_str());
                /* 
                 here i could have used an OOP implementation , 
                 With a package object and stuff , but its complicated , really ineficcient
                */
                // doing some bad stuff

                std::string packagePath = soviet::format("%s/%s.spm",soviet::vars.SPM_DIR.c_str(),param.c_str());
               
                if (std::filesystem::exists(packagePath))
                {
                        std::ifstream f(packagePath);
                        if (f.is_open()) {
                            std::cout << f.rdbuf();
                        }
                        std::cout << "\n";
                            
                }
                else
                {
                    soviet::msg(soviet::level::FATAL, "Package %s not found!", param.c_str());
                }

            }
            break;
        case TEST :
            // This is just for testing purposes
            // it wont be used in the final version
            soviet::arch2spm ("../neofetch-git/PKGBUILD");
            break;
        default :
            soviet::msg(soviet::level::FATAL, "Action error! ...\n");
            break;
    }


    //Returning 0 means the program ran successfully
    // 
    return 0;
}
float version()
{
    return (float)RELEASE;
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

