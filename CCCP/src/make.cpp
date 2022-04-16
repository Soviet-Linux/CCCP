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
void make_pkg (const std::string& PName, const std::string& build_info, const std::string& WORK_DIR)
{

    const std::string& build_cmd = "BUILD_ROOT="+ WORK_DIR +"build\n( cd "+ WORK_DIR +"sources/"+ PName +"* && "+ build_info +" )";
    std::cout << build_cmd << std::endl;
    
    system(build_cmd.c_str());
    system(("rm -rf " + WORK_DIR + "sources/" + PName + "*").c_str());
}
void download_pkg (const std::string& download_info, const std::string& WORK_DIR)
{
    const std::string& download_cmd = "( cd "+ WORK_DIR +"sources/ && "+download_info+" )";

    std::cout << download_cmd << "\n";
    system(download_cmd.c_str());
}
// This function is moving the binaries to the correct locations
void move_binaries(const std::string& BUILD_DIR ,const std::string& ROOT)
{
    //moving binaries to their install location on the system
    std::string move_cmd = "cp -rvl " + BUILD_DIR + "* " + ROOT + "\n ";
    std::cout << move_cmd << "\n";
    system(move_cmd.c_str());
    system(("rm -rf " + BUILD_DIR + "*").c_str());
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
    data.special_info = pkg_info["info"]["special"];
    for (int i = 0; i < pkg_info["dependencies"].size(); i++)
    {
        data.dependencies.push_back(pkg_info["dependencies"][i]);
    }
    if (data.type == "src")
    {
        data.download_info = pkg_info["info"]["download"];
        data.build_info = pkg_info["info"]["build"];
    }
    else if (data.type == "local")
    {
        data.build_info = pkg_info["info"]["build"];
    }
    return data;
}
// changing source spm file to bin spm file
void bin_spm (const std::string& in_path , const std::string& out_path)
{
    std::ifstream file_spm((in_path).c_str(), std::ios::in);
    std::stringstream buffer;
    buffer << file_spm.rdbuf();
    file_spm.close();
    //parsing json data
    auto pkg_info = json::parse(buffer.str());
    pkg_info["type"] = "bin";
    pkg_info["info"]["build"] = "";
    pkg_info["info"]["download"] = "";
    std::ofstream file_spm_bin((out_path).c_str(), std::ios::out);
    file_spm_bin << pkg_info.dump(4);
    file_spm_bin.close();
    
}

