#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <map>


#include "../../include/shared.h"

#define RELEASE 0.25

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

    // new argument system
    std::map<std::string,actionList> long2action =
    {
        {"help", HELP},
        {"print",PRINT},
        {"package",INSTALL_LOCAL},
        {"install",INSTALL_FROM_REPO},
        {"check",CHECK},
        {"list",LIST},
        {"remove",REMOVE},
        {"archive",CREATE_ARCHIVE},
        {"update",UPDATE},
        {"clean",CLEAN},
        {"sync",SYNC},
        {"test",TEST},
        {"build",CREATE_BINARY},
        {"compatible",INSTALL_COMPATIBLE},
        {"convert",TO_SPM},
        {"aur",INSTALL_FROM_AUR}



    };
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
                if (longOption == "debug")
                {
                    FConf.DEBUG = 3;
                }
                else if (longOption == "verbose")
                {
                    FConf.QUIET = false;
                }
                else if (longOption == "overwrite")
                {
                    FConf.OVERWRITE = true;
                }
                else if (longOption == "version")
                {
                    std::cout << "CCCP C++ Frontend v" << RELEASE << std::endl;
                    std::cout << "Libspm C++ Library v" << version() << std::endl;
                    return 0;
                }
                else {
                    action = long2action[longOption];
                }
                


            }
            else  
            {
                std::cout << "Sorry , you cant use [-?] args for now , but we are working on it ." << std::endl;
                /*
                for (int i = 1;i < option.size();i++)
                {

                }
                */
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
