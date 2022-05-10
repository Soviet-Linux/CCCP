#include <string>
#include <fstream>
#include <iostream>

#include "../lib/nlohmann/json.hpp"
using nlohmann::json;
/*
This file contains several functions that are used to store , remove and manipulate installed package data.
Currently the data is stored in a json file.
This is not optimal , in fact it is reaaallly bad , but I dont know database stuff.
So every package name and version is stored , also the number of packages
If someone wants to chnage it , you just have to rewrite theses functions, dont touch other files.
I you want to do something , you can use sqlite3 or any other database.

I really dont like that code so i wont comment. 
*/

int init_data (const std::string& data_path)
{
    auto basic_json = json::parse("{\"package_list\" :[],\"packages\" : 0}");
    std::cout << basic_json.dump(4) << std::endl;
    //write the json to data_path
    std::ofstream file_spm((data_path).c_str(), std::ios::out);
    file_spm << basic_json.dump(4);
    file_spm.close();

    return 0;
}
int add_data (const std::string& data_path, const std::string& pkg_name, const std::string& pkg_version)
{
    std::ifstream file_spm((data_path).c_str(), std::ios::in);
    std::stringstream buffer;
    buffer << file_spm.rdbuf();
    file_spm.close();
    //parsing json data
    auto pkg_info = json::parse(buffer.str());
    json o;
    o["name"] = pkg_name;
    o["version"] = pkg_version;
    pkg_info["package_list"].push_back(o);
    pkg_info["packages"] = int(pkg_info["packages"]) + 1;
    std::ofstream file_data_new((data_path).c_str(), std::ios::out);
    file_data_new << pkg_info.dump(4);
    file_data_new.close();
    return 0;
}
