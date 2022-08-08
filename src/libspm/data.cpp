#include <string>
#include <fstream>
#include <iostream>



//class thing
#include "../../include/libspm.h"

/*
This file contains several functions that are used to store , remove and manipulate installed package data.
Currently the data is stored in a json file.
This is not optimal , in fact it is reaaallly bad , but I dont know database stuff.
So every package name and version is stored , also the number of packages
If someone wants to chnage it , you just have to rewrite theses functions, dont touch other files.
I you want to do something , you can use sqlite3 or any other database.

I really dont like that code so i wont comment. 
*/
int soviet::init_data()
{
    auto basic_json = json::parse("{\"package_list\" :[],\"packages\" : 0}");
    msg(DBG2, "Base data file loaded : %s", basic_json.dump().c_str());
    //write the json to data_path
    std::ofstream file_spm((vars.INSTALLED_FILE).c_str(), std::ios::out);
    file_spm << basic_json.dump(4);
    file_spm.close();

    return 0;
}
int soviet::package::add_data ()
{
    msg(INFO, "Adding %s data to %s", name.c_str(), vars.INSTALLED_FILE.c_str());
    std::ifstream file_spm((vars.INSTALLED_FILE).c_str(), std::ios::in);
    std::stringstream buffer;
    buffer << file_spm.rdbuf();
    file_spm.close();
    //parsing json data
    auto pkg_info = json::parse(buffer.str());
    json o;
    o["name"] = name;
    o["version"] = version;
    o["type"] = type;
    pkg_info["package_list"].push_back(o);
    pkg_info["packages"] = int(pkg_info["packages"]) + 1;
    std::ofstream file_data_new((vars.INSTALLED_FILE).c_str(), std::ios::out);
    file_data_new << pkg_info.dump(4);
    file_data_new.close();
    return 0;
}
int soviet::package::remove_data ()
{
    std::ifstream file_spm((vars.INSTALLED_FILE).c_str(), std::ios::in);
    std::stringstream buffer;
    buffer << file_spm.rdbuf();
    file_spm.close();
    //parsing json data
    auto pkg_info = json::parse(buffer.str());
    for (int i = 0; i < pkg_info["package_list"].size(); i++)
    {
        std::string pkg_name = pkg_info["package_list"][i]["name"];
        if (pkg_name  == name )
        {
            pkg_info["package_list"].erase(pkg_info["package_list"].begin() + i);
            pkg_info["packages"] = int(pkg_info["packages"]) - 1;
            break;
        }
    }
    std::ofstream file_data_new((vars.INSTALLED_FILE).c_str(), std::ios::out);
    file_data_new << pkg_info.dump(4);
    file_data_new.close();
    return 0;
}
