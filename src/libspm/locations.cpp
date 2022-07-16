#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "stdio.h"


// class stuff
#include "../../include/libspm.hpp"

void soviet::package::get_locations()
{
    //Get package file location
    std::string files_location_cmd = soviet::format("( cd %s && find . -type f | cut -c2- ) ", vars.BUILD_DIR.c_str());
    /*
    This way of getting locations is pretty bad , i should wite a proper way to do it
    But it works so its ok for now 
    */    
    msg(DBG2, "Getting files locations with %s ", files_location_cmd.c_str());

   
    // execute the commands
    std::string files_locations = soviet::exec(files_location_cmd.c_str());
    // print the locations
    msg(DBG3, "Package locations : \n Files : %s ", files_location_cmd.c_str());

    // Parsing everything ad storing it in a vector
    std::string line;
    std::stringstream files_ss(files_locations);  
    while (std::getline(files_ss, line)) {
        locations.push_back(line);
    } 


}