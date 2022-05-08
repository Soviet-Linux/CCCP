#include <iostream>
#include <string>
#include <vector>

#include "../include/locations.h"
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
            std::cout << " " << ent->d_name << "\n";
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

int main()
{
    std::string location = "../../";
    std::cout << location << std::endl;
    get_locations(location);
}