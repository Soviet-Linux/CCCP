#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "../include/misc.h"
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

int check_dependencies (const nlohmann::basic_json<>&, const std::string& DATA_DIR) 
{

    return 1;
}

std::vector<std::string> init_pkg_list(const std::string& DATA_DIR)
{
    
}