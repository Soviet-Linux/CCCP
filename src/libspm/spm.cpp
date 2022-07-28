#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>


#include "../../lib/nlohmann/json.hpp"

// class thing
#include "../../include/libspm.hpp"

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
    msg(DBG1,"Storing package spm to %s",spm_out.c_str());
    msg(DBG2,"Parsing %s",spm_path.c_str());
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
    msg(DBG2,"Parsing %s",spm_path.c_str());
    name = pkg_info["name"];
    type = pkg_info["type"];
    version = pkg_info["version"];
    msg(DBG3,"Adding dependencies");
    for (int i = 0; i < pkg_info["dependencies"].size(); i++)
    {
        dependencies.push_back(pkg_info["dependencies"][i]);
    }
    msg(DBG3,"adding url");
    if (!pkg_info["url"].is_null())
    {
        url = pkg_info["url"];
    }
    msg(DBG3,"launching cool code");
    // im really proud of the code below , it looks like some professional  stuff
    std::vector<std::string> infos {"download","prepare","make","test","install","special"};
    for (int i = 0; i < infos.size(); i++)
    {
        if (pkg_info["info"][infos[i]].is_null())
        {
            msg(DBG3,"%s is null",infos[i].c_str());
            continue;
        }
        else
        {
            info.insert(std::pair<std::string,std::string>(infos[i],pkg_info["info"][infos[i]]));
            msg(DBG3,"setting %s , to %s",info[infos[i]].c_str(),pkg_info["info"][infos[i]].dump().c_str());
        }
    }
    msg(DBG3,"ending cool code");
    // end of the cool code -^
    msg(DBG3,"adding locations");
    for (int i = 0; i < pkg_info["locations"].size(); i++)
    {
        locations.push_back(pkg_info["locations"][i]);
    }
    
    msg(DBG1,"Package %s initialized successfully !",name.c_str());
    return 1;
}
