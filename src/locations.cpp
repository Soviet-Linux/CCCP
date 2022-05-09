#include <fstream>
#include <string>
#include <vector>

std::vector<std::string> get_locations(const std::string &PATH)
{
    //declaring output vector
    std::vector<std::string> locations;

    std::string temp_file = "/tmp/temp_loc.txt";
    //Get package file location
    std::string location_cmd = "( cd " + PATH + " && find . -type f | cut -c2- > " + temp_file + " && find . -type d | cut -c2- | tac  | sed '/^$/d' >> " + temp_file + " )";
    //std::cout << location_cmd << std::endl;
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
    return locations;
}