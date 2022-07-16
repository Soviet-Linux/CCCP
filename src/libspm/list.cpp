#include <vector>
#include <iostream>
#include <string>

// class stuff
#include "../../include/libspm.hpp"
void soviet::listPkgs() {
    //open DATA_FILE
    std::ifstream file_spm((vars.INSTALLED_FILE).c_str(), std::ios::in);
    std::stringstream buffer;
    buffer << file_spm.rdbuf();
    file_spm.close();
    //parsing json data
    auto pkg_info = json::parse(buffer.str());
    for (int i = 0; i < pkg_info["package_list"].size(); i++) {
        std::string Pname = pkg_info["package_list"][i]["name"];
        std::string Ptype = pkg_info["package_list"][i]["type"];
        std::string Pversion = pkg_info["package_list"][i]["version"];
        std::string pkg_name = soviet::format("%s-%s (%s)",Pname.c_str(),Pversion.c_str(),Ptype.c_str());
        std::cout << pkg_name << std::endl;
    }
    std::cout << "Total packages: " << pkg_info["package_list"].size() << std::endl;
    
}