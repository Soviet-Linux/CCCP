#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "../include/nlohmann/json.hpp"

using nlohmann::json;

#include "../include/make.h"

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

int check_dependencies (const nlohmann::basic_json<>& dependencies, const std::string& DATA_DIR) 
{
    std::cout << dependencies.dump() << "\n";
    return 1;
}
pkg_data open_spm (const std::string& PPath)
{
    std::ifstream file_spm((PPath).c_str(), std::ios::in);
    std::stringstream buffer;
    buffer << file_spm.rdbuf();
    //parsing json data
    auto pkg_info = json::parse(buffer.str());
    std::cout  << pkg_info << "\n";
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