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
        std::string cmd = "cp -rflv " + BUILD_DIR + "/*" + " " + ROOT;
        system(cmd.c_str());
        
        
    }
}