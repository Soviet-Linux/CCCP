#include <string>
#include <vector>
#include <iostream>

// C headers (becausee c is better)
#include "dirent.h"

std::vector<std::string> get_locations(const std::string& BUILD_DIR)
{
    std::vector<std::string> locations;

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (BUILD_DIR.c_str())) != NULL) 
    {
        while ((ent = readdir (dir)) != NULL) 
        {
            locations.push_back(ent->d_name);
        }
        closedir (dir);
        
    } 
    else 
    {
        std::cout << "could not open directory\n";
        exit(1);

    }
    return locations;  
}