#include <string>
#include <unistd.h>
#include <vector>
#include <stdio.h>
#include <iostream>

// class stuff
#include "../../include/libspm.hpp"


//checking if package is installed and untouched
bool soviet::package::check ()
{
    // checkinig if package data file exists
    std::cout << dataSpmPath << std::endl;
    if (access(dataSpmPath.c_str(),F_OK)){
        std::cout << "Package data file found!" << std::endl;
        return false;
    }
    else
    {
        msg(level::INFO, "Package data file exists, good!");  
        // Pasing data from the spm file
        var_spm(dataSpmPath);
    } 
    // check if the data["location "] is not empty
    //it could be empty because of a failed install
    if (locations.empty())
    {
        msg(level::DBG, "Package data file is empty, something is wrong!");
        return false;
    }
    //check all the files in the data["locations"]
    for (int i = 0; i < locations.size(); i++)
    {
        /*
            Checking if every file installed by the package is still there 
            This is a bad way to do  this but i dont have a better idea 
            Again , if you have an idea DM me on discord 
        */

         msg(level::INFO, "Checking %s", locations[i].c_str());
         if (!access(locations[i].c_str(),F_OK)) 
         {
             msg(level::DBG, "%s exists!", locations[i].c_str());
         }
         else {
                msg(level::ERROR, "%s does not exist!", locations[i].c_str());
                return false;
         }
    }
    return true;
}
