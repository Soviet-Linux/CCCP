#include <string>
#include <iostream>
#include <fstream>


#include "../lib/nlohmann/json.hpp"

//using json = nlohmann::json;
using nlohmann::json;

int open_spm (const std::string& PPath)
{
    std::ifstream file_spm((PPath).c_str(), std::ios::in);
    std::stringstream buffer;
    buffer << file_spm.rdbuf();
    //parsing json data
    auto pkg_info = json::parse(buffer.str());

    name = pkg_info["name"];
    type = pkg_info["type"];
    version = pkg_info["version"];
    special_info = pkg_info["info"]["special"];
    for (int i = 0; i < pkg_info["dependencies"].size(); i++)
    {
        dependencies.push_back(pkg_info["dependencies"][i]);
    }

    if (type == src)
    {
        prepare_info = pkg_info["info"]["prepare"];
        build_info = pkg_info["info"]["make"];
        test_info = pkg_info["info"]["test"];
        install_info = pkg_info["info"]["install"];
        
    }
    else if (type == bin)
    {
        for (int i = 0; i < pkg_info["locations"].size(); i++)
        {
        locations.push_back(pkg_info["locations"][i]);
        }
    }
    return 1;
    
}
//This fucntion is very important , it will store the install location data to the "DB"
void store_spm (const std::string& PPath,const std::string& out_path)
{
    std::string temp_file = "/tmp/temp_loc.txt";
    std::cout << "Storing location in spm file" << std::endl;
    std::ifstream file_spm((PPath).c_str(), std::ios::in);
    std::stringstream buffer;
    buffer << file_spm.rdbuf();
    file_spm.close();
    //parsing json data
    auto pkg_info = json::parse(buffer.str());
    //change package type if its a binary
    if (type == bin)
    {
        pkg_info["type"] = "bin";
    }
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
            pkg_info["locations"].push_back(line);
        }
        data_file.close();
    }
    //removing temp file 
    // TODO: comment this better
    system(("rm "+ temp_file).c_str());
    //Writing the data to a file 
    std::ofstream file_spm_out((out_path).c_str(), std::ios::out);
    file_spm_out << pkg_info.dump(4);
    file_spm_out.close();
}