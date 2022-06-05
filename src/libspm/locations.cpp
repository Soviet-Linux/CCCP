#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "stdio.h"


// class stuff
#include "../../include/cccp.hpp"

void soviet::package::get_locations()
{
    //Get package file location
    std::string files_location_cmd = soviet::format("( cd %s && find . -type f | cut -c2- ) ", BUILD_DIR.c_str());
    std::string dirs_location_cmd = soviet::format("( cd %s && find . -type d | cut -c2- | tac | sed '/^$/d')", BUILD_DIR.c_str());
    if (soviet::DEBUG) std::cout << files_location_cmd << dirs_location_cmd << std::endl;
    // execute the commands
    std::string files_locations = soviet::exec(files_location_cmd.c_str());
    std::string dirs_locations = soviet::exec(dirs_location_cmd.c_str());
    // print the locations
    if (soviet::DEBUG) std::cout << "Files locations : " << files_locations << std::endl;
    if (soviet::DEBUG) std::cout << "Dirs locations : " << dirs_locations << std::endl;

    // Parsing everything ad storing it in a vector
    std::string line;
    std::stringstream files_ss(files_locations);  
    std::stringstream dirs_ss(dirs_locations);
    while (std::getline(files_ss, line)) {
        locations.push_back(line);
    } 
    while ( getline (dirs_ss,line) ) {
        locations.push_back(line);
    }


}