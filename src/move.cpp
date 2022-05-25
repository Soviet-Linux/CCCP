#include <stdio.h>
#include <vector>
#include <string>

// class stuff
#include "../include/cccp.hpp"

// This function is moving the binaries to the correct locations
void soviet::package::move_binaries()
{
    //moving binaries to their install location on the system
    //loop through the locations and move them to the correct location
    for (int i = 0; i < locations.size(); i++)
    {
        // Please someone find a better solution than a system call
        //move all files from BUILD_DIR to ROOT
        std::string move_cmd = format("shopt -s extglob ;cp -rfl  !(%s.spm) %s/* %s",name.c_str(),BUILD_DIR.c_str(),ROOT.c_str());
        if (DEBUG) std::cout << "Executing move cmd :" << move_cmd << std::endl;
        system(move_cmd.c_str());
        
        
    }
}