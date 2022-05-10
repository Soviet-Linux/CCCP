#include <string>
#include <unistd.h>
#include <vector>
#include <stdio.h>
#include <iostream>


//checking if package is installed and untouched
bool check_pkg (const std::string& PName,bool DEBUG)
{
    std::string PPath = DATA_DIR + PName + ".spm";
    // Pasing data from the spm file
    open_spm(PPath);
    // check if the data["location "] is not empty
    //it could be empty because of a failed install
    if (locations.empty())
    {
        if (DEBUG) std::cout << "It may be some serious shit in the package , do something !!\n";
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
         if (DEBUG) std::cout << "Checking " << data.locations[i] << std::endl;
         if (access(locations[i]),F_OK) 
         {
             if (DEBUG) std::cout << locations[i] << " exists !" << std::endl;
         }
         else {
                std::cout << locations[i] << " does not exist !" << std::endl;
                return false;
         }
    }
    return true;
}
