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
        // C is better (Why this fucking function is called rename and not move ?!)
        rename(locations[i].c_str(), (ROOT + locations[i]).c_str());
        
    }
}