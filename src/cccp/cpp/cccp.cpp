#include <iostream>
#include <vector>
#include <string>


#include <unistd.h>

#define RELEASE 0.25

#include "../../../include/globals.h"

extern "C" float version();
extern "C" int strcpa(char** dest,const char* value);

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

    DEBUG = 3;

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
                if (longOption == "version")
                {
                    std::cout << "CCCP C++ Frontend v" << RELEASE << std::endl;
                    std::cout << "Libspm C++ Library v" << version() << std::endl;
                    return 0;
                }
                else if (longOption == "test")
                {
                    strcpa(NULL,NULL);
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
                continue;
            }
        }
    }

    return 0;
}
