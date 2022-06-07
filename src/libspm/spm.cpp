#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>


#include "../../lib/nlohmann/json.hpp"

// class thing
#include "../../include/cccp.hpp"

//using json = nlohmann::json;
using nlohmann::json;

nlohmann::json soviet::package::open_spm (const std::string& PPath)
{
    if (access(PPath.c_str(), F_OK))
    {
        std::cout << "SPM file not found!" << std::endl;
        return json();
    }
    std::ifstream file_spm((PPath).c_str(), std::ios::in);
    std::stringstream buffer;
    buffer << file_spm.rdbuf();
    //parsing json data
    auto pkg_info = json::parse(buffer.str());

    return pkg_info;
    
}
// This function is very important , it will store the install location data to the "DB"
void soviet::package::store_spm (const std::string& spm_path,const std::string& spm_out)
{
    if (DEBUG) std::cout << "Storing package spm to " << spm_out << std::endl;
    if (DEBUG) std::cout << "parsing : " << spm_path << std::endl;
    auto pkg_info = open_spm(spm_path);
    //change package type if its a binary

    if (type == "src")
    {
        for (int i = 0; i < locations.size(); i++)
        {
            pkg_info["locations"].push_back(locations[i]);
        }
    }
    if (type == "bin")
    {
        pkg_info["type"] = "bin";
    }
    
    //Writing the data to the package spm file in data_dir (dataSpmPath)
    std::ofstream file_spm_out((spm_out).c_str(), std::ios::out);
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
    for (int i = 0; i < pkg_info["locations"].size(); i++)
    {
        locations.push_back(pkg_info["locations"][i]);
    }
    if (soviet::DEBUG) std::cout << "Package " << name << " initialized successfully !" << std::endl;
    return 1;
}