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


std::string soviet::cwd = getcwd(NULL,0);



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
                pkg.install();
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
            break;
        case CHECK :
            for (auto & param : parameters)
            {
                soviet::msg(soviet::level::INFO, "Checking %s", param.c_str());

                soviet::package pkg;
                pkg.name = param;
                pkg.dataSpmPath = soviet::format("%s/%s.spm",soviet::vars.SPM_DIR.c_str() , pkg.name.c_str());
                // debug
                soviet::msg(soviet::level::DBG1, "Launching check for %s with %s", pkg.name.c_str(), pkg.dataSpmPath.c_str());
                if (pkg.check())
                {
                    soviet::msg(soviet::level::INFO, "Package %s is installed and good!", param.c_str());

                }
                else 
                {
                    soviet::msg(soviet::level::ERROR, "Package %s is not installed or corrupted!", param.c_str());
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
                pkg.install();
                std::filesystem::remove_all(soviet::vars.TMP_DIR);
            }
            break;
        case CREATE_BINARY:
            if (parameters.empty())
            {      
                soviet::msg(soviet::level::FATAL, "No packages specified! Terminating...");
            }         
            for (auto & param : parameters)
            {

                soviet::msg(soviet::level::INFO, "Creating binary package from %s", param.c_str());

                soviet::package pkg = soviet::parseFileName(param);
                
                // check if package is source
                if (pkg.type != "src") soviet::msg(soviet::FATAL,"Specified package isn't a source spm package !" );

                soviet::msg(soviet::level::DBG1, "Launching creation with %s", pkg.packagePath.c_str());
                
                pkg.createBinary(soviet::format("%s/%s.bin.spm.tar.gz",soviet::cwd.c_str(),pkg.name.c_str()));
            }
            break;
        case CREATE_ARCHIVE:
            if (parameters.empty())
            {      
                soviet::msg(soviet::level::FATAL, "No packages specified! Terminating...");
            }         
            for (auto & param : parameters)
            {

                soviet::msg(soviet::level::INFO, "Creating source package archive from %s", param.c_str());
                
                soviet::package pkg;
                
                if (param.substr(param.size() -3 ,param.size()) != "spm")
                {
                    msg(soviet::FATAL,"This is not a valid package.");
                } 

                pkg.packagePath = param;

                soviet::msg(soviet::level::DBG1, "Launching creation with %s", pkg.packagePath.c_str());

                pkg.createArchive(soviet::format("%s/%s.bin.spm.tar.gz",soviet::cwd.c_str(),pkg.name.c_str()),{});
            }
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
        case TO_SPM:
            for (auto & param : parameters)
            {
                std::string FileName = param.substr(param.find_last_of("/")+1,param.size());
                if (FileName == "PKGBUILD")
                {
                    soviet::msg(soviet::INFO, "Tranforming %s to spm in ARCHLINUX compatibility mode", param.c_str());
                    soviet::arch2spm (param,"");

                }
                else
                {
                    soviet::msg(soviet::level::FATAL, "%s files  arent compatible , sorry", FileName.c_str());
                }
            }
            break;
        case INSTALL_COMPATIBLE :
            for (auto & param : parameters)
            {
                std::string FileName = param.substr(param.find_last_of("/")+1,param.size());
                if (FileName == "PKGBUILD")
                {
                    std::string AbsPath = soviet::format("%s/%s",soviet::cwd.c_str(),param.c_str());
                    soviet::msg(soviet::INFO, "installing %s in archlinux compatibility mode", param.c_str());
                    soviet::package ArchPkg;
                    ArchPkg.type = "src";
                    ArchPkg.name = soviet::arch2spm(param, soviet::format("mkdir $NAME-$VERSION && cp %s .",AbsPath.c_str()));
                    msg(soviet::INFO,"PKGBUILD succesfully converted !");
                    ArchPkg.packagePath = soviet::format("%s/%s.spm",soviet::cwd.c_str(),ArchPkg.name.c_str());
                    ArchPkg.dataSpmPath = soviet::format("%s/%s.spm",soviet::vars.SPM_DIR.c_str(),ArchPkg.name.c_str());
                    ArchPkg.install();

                }
                else
                {
                    soviet::msg(soviet::level::FATAL, "%s files  arent compatible , sorry", FileName.c_str());
                }
            }
        case TEST :
            // This is just for testing purposes
            // it wont be used in the final version
            
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

