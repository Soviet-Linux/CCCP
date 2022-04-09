#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// Json parsing library
#include "../include/nlohmann/json.hpp"

//using json = nlohmann::json;
using nlohmann::json;

//including header file
#include "../include/make.h"

// This function is downloading and building the packages
void make_pkg (const std::string& PName, const std::string& download_info, const std::string& build_info, const std::string& CURRENT_DIR)
{
    const std::string& download_cmd = "( cd "+ CURRENT_DIR +"sources/ && "+download_info+" )";

    std::cout << download_cmd << "\n";
    system(download_cmd.c_str());

    const std::string& build_cmd = "BUILD_ROOT="+ CURRENT_DIR +"build\n( cd "+ CURRENT_DIR +"sources/"+ PName +" && "+ build_info +" )";
    std::cout << build_cmd << std::endl;
    
    system(build_cmd.c_str());
    system(("rm -rf " + CURRENT_DIR + "sources/" + PName).c_str());
}
// This function is moving the binaries to the correct locations
void move_binaries(const std::string& BUILD_DIR ,const std::vector<location>& install_info)
{
    //moving binaries to their install location on the system
    for (int i = 0; i < install_info.size(); i++)
    {
        std::string install_cmd = "mv " + BUILD_DIR + install_info[i].file + " " + install_info[i].destination; 
        std::cout << install_cmd << std::endl;
        system(install_cmd.c_str());
    }
}
// This function will check if all dependencies of a package are installed
int check_dependencies (const std::vector<std::string>& dependencies, const std::string& DATA_DIR) 
{
    //TODO : check if the dependencies are installed
    //
    //Print the dependencies
    std::cout << "Dependencies : ";
    for (int i = 0; i < dependencies.size(); i++)
    {
        std::cout << dependencies[i] << " ";
    }
    std::cout << std::endl;
    return 1;
}
// This function will open the spm file , parse the json and return the data
pkg_data open_spm (const std::string& PPath)
{
    std::ifstream file_spm((PPath).c_str(), std::ios::in);
    std::stringstream buffer;
    buffer << file_spm.rdbuf();
    //parsing json data
    auto pkg_info = json::parse(buffer.str());
    pkg_data data;
    data.name = pkg_info["name"];
    data.type = pkg_info["type"];
    data.version = pkg_info["version"];
    for (int i = 0; i < pkg_info["dependencies"].size(); i++)
    {
        data.dependencies.push_back(pkg_info["dependencies"][i]);
    }
    data.download_info = pkg_info["info"]["download"];
    data.build_info = pkg_info["info"]["build"];
    for (int i = 0; i < pkg_info["install"].size(); i++)
    {
        data.install_info.push_back({pkg_info["install"][i]["file"], pkg_info["install"][i]["destination"]});
    }
    return data;
}