/*
Guys , this is the cccp , a communist package manager.
My function and variables names are awful , I know.
Good luck to everyone that will try to understand this shit.
Thank you for your help :)
*/


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
        std::cout << "No arguments given. Terminating.\n";
        exit(1);
    }

    //parse agruments
    std::vector<soviet::action> actions = soviet::parse(argc, argv);

    //execute actions
    for (int i = 0; i < actions.size(); i++) 
    {
        switch (actions[i])
        {
            case soviet::INSTALL:
                std::cout << "Installing package...\n";
                break;
            default :
                std::cout << "Parsing shit. Fix this !\n";
        }
    }

    //Returning 0 means the program ran successfully
    // 
    return 0;
}
