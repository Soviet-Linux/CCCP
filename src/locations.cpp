#include <fstream>
#include <string>
#include <vector>

// class stuff
#include "../include/cccp.hpp"

void soviet::package::get_locations()
{
    std::string temp_file = "/tmp/locations.cccp.tmp.txt";
    //Get package file location
    std::string location_cmd = soviet::format("( cd %s && find . -type f | cut -c2- > %s && find . -type d | cut -c2- | tac | sed '/^$/d' >> %s )", BUILD_DIR.c_str(), temp_file.c_str(), temp_file.c_str());
    if (soviet::DEBUG) std::cout << location_cmd << std::endl;
    system(location_cmd.c_str());
    //also the temp.txt file is a little hacky i think
    //Add the package locations
    std::string line;
    std::ifstream data_file ((temp_file).c_str());
    //adding the location the the location list
    if (data_file.is_open())
    {
        //reading the command output from a file
        while ( getline (data_file,line) )
        {
            locations.push_back(line);
        }
        data_file.close();
    }
}