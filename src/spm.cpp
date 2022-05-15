#include <string>
#include <iostream>
#include <fstream>


#include "../lib/nlohmann/json.hpp"

// class thing
#include "../include/cccp.hpp"

//using json = nlohmann::json;
using nlohmann::json;

nlohmann::json soviet::package::open_spm (const std::string& PPath)
{
    std::ifstream file_spm((PPath).c_str(), std::ios::in);
    std::stringstream buffer;
    buffer << file_spm.rdbuf();
    //parsing json data
    auto pkg_info = json::parse(buffer.str());

    return pkg_info;
    
}
// This function is very important , it will store the install location data to the "DB"
void soviet::package::store_spm (const std::string& spm_path,const std::string& out_path)
{
    std::string temp_file = "/tmp/spm.cccp.tmp";
    std::cout << "Storing location in spm file" << std::endl;
    std::ifstream file_spm((spm_path).c_str(), std::ios::in);
    std::stringstream buffer;
    buffer << file_spm.rdbuf();
    file_spm.close();
    //parsing json data
    auto pkg_info = json::parse(buffer.str());
    //change package type if its a binary
    if (type == "bin")
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
    //Writing the data to the package spm file in data_dir (SPath)
    std::ofstream file_spm_out((dataSpmPath).c_str(), std::ios::out);
    file_spm_out << pkg_info.dump(4);
    file_spm_out.close();
}
int soviet::package::var_spm(const std::string& spm_path)
{
    auto pkg_info = open_spm(spm_path);
    name = pkg_info["name"];
    type = pkg_info["type"];
    version = pkg_info["version"];
    special_info = pkg_info["info"]["special"];
    for (int i = 0; i < pkg_info["dependencies"].size(); i++)
    {
        dependencies.push_back(pkg_info["dependencies"][i]);
    }
    prepare_info = pkg_info["info"]["prepare"];
    build_info = pkg_info["info"]["make"];
    test_info = pkg_info["info"]["test"];
    install_info = pkg_info["info"]["install"];
    if (type == "bin")
    {
        for (int i = 0; i < pkg_info["locations"].size(); i++)
        {
        locations.push_back(pkg_info["locations"][i]);
        }
    }
    //initialize variables
    dataSpmPath = SPM_DIR + name + ".spm";
    if (soviet::DEBUG) std::cout << "Package " << name << " initialized successfully !" << std::endl;
    return 1;
}