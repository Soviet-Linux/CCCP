#include <stdio.h>
#include <vector>
#include <string>
#include <stdlib.h>
#include <unistd.h>
// class stuff
#include "../../include/libspm.hpp"

// This function is moving the binaries to the correct locations
void soviet::package::move_binaries()
{
    /*

    //This is old way of moving binaries, it works but is not very clean

    std::string move_cmd = format("cp -rfl %s/* %s",BUILD_DIR.c_str(),ROOT.c_str());
    msg(DBG3,"Executing move command : %s",move_cmd.c_str());
    if (system(move_cmd.c_str()) != 0)
    {
        std::cout << "Error moving binaries, its is bad !" << std::endl;
        exit(1);
    }
    */

    // check if all locations are empty
    msg(DBG1,"Checking if all locations are empty");
    /*
     For now , this checking part is completely useless 
     i plan to use it to do some checks on the system.
     I will leave it like that for now 
    */
    for (int i = 0; i < locations.size(); i++)
    {
        msg(DBG3,"Checking if %s is empty",locations[i].c_str());
        if (access(locations[i].c_str(),F_OK) == 0)
        {
            msg(DBG3, "moving %s/%s to %s", vars.BUILD_DIR.c_str(),locations[i].c_str(),locations[i].c_str());
            // now that we know it is empty , mov the stuff
            rename(format("%s/%s",vars.BUILD_DIR.c_str(),locations[i].c_str()),locations[i].c_str());

        }
        else 
        {

            msg(ERROR,"Error, %s is already here",locations[i].c_str());

            if (vars.OVERWRITE) 
            {
                rename(format("%s/%s",vars.BUILD_DIR.c_str(),locations[i].c_str()),locations[i].c_str());
            }
            else {
                msg(FATAL, "Terminating the program");
            }
        }
    }


}