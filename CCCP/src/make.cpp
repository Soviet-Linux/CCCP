#include <fstream>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <vector>
#include <filesystem>




//including header file
#include "../include/make.h"
#include "../include/pkg.h"



// This function is moving the binaries to the correct locations
void move_binaries(const std::string& BUILD_DIR ,const std::string& ROOT)
{
    //moving binaries to their install location on the system
    std::string move_cmd = "cp -rfl " + BUILD_DIR + "* " + ROOT + "\n ";
    system(move_cmd.c_str());
    std::filesystem::remove(BUILD_DIR + "*");
}
// This function will check if all dependencies of a package are installed
int check_dependencies (const std::vector<std::string>& dependencies, const std::string& DATA_DIR) 
{
    //TODO : check if the dependencies are installed
    //
    //Print the dependencies
    if (dependencies[0] == "base")
    {
        std::cout << "this packages requires the base system , no dependencies checking.\n";
        return 1;
    }
    else {
        for (int i = 0; i < dependencies.size(); i++)
        {
            // TODO: implement vesrion checking
            if (std::filesystem::exists(DATA_DIR + dependencies[i] + ".spm"))
            {
                std::cout << "dependency " << dependencies[i] << " is installed.\n";
            }
            else {
                std::cout << "dependency " << dependencies[i] << " is not installed.\n";
                return 0;
            }
        }
    }
   
    return 1;
}

