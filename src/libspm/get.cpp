#include <curl/curl.h>
#include <iostream>
#include <ostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
//include json lib
#include "../../lib/nlohmann/json.hpp"

 // class stuff
#include "../../include/libspm.hpp"

void soviet::package::get()
{
    packagePath = soviet::format("%s/%s.%s.spm.tar.gz",vars.TMP_DIR.c_str(), name.c_str(), type.c_str());
    

    // I commented this part because the soviet system im working on right now doesnt support curl 
    // I will add it later whan the rest of the stuff is ready 
    
    // check if ALL_FILE exists
    if (access((vars.ALL_FILE).c_str(),F_OK))
    {
        // This is the first tim is use a ''' do {...} while(...) ''' loop
        do
        {
            std::cout << "There are no package data file , do you want to download it ? [y/n]";
            std::cin >> type;
        }
        while(  type != "y" && type != "n");
        if (type == "n")
        {
            std::cout << "Exiting" << std::endl;
            return;
        }  
        else if (type == "y")
        {
            sync();
        } 
        else 
        {
            std::cout << "something went wrong" << std::endl;
        }

        
    }
    if (vars.DEBUG) std::cout << "Loading " << vars.ALL_FILE << std::endl;
    // verify if package exists
    // parse ALL_FILE and check if the package is there
    std::ifstream file_spm((vars.ALL_FILE).c_str(), std::ios::in);
    std::stringstream buffer;
    buffer << file_spm.rdbuf();
    //parsing json data
    auto all_pkgs = json::parse(buffer.str());
    msg(DBG1,"Parsing %s" ,vars.ALL_FILE.c_str());
    // check if the package is there
    // this solution is pretty bad
    // TODO : improve it
    for (int i = 0; i < all_pkgs["all"].size(); i++)
    {
        std::string pkg_name = all_pkgs["all"][i]["name"];
        if (pkg_name  == name )
        {
            type = all_pkgs["all"][i]["type"];
            version = all_pkgs["all"][i]["version"];
            break;
        }
    }
    if (type == "")
    {
        msg(FATAL,"Package %s not found",name.c_str());
        return;
    }
    else {
        msg(INFO,"Package %s %s %s found",name.c_str(),version.c_str(),type.c_str());
    }
    
    

    msg(DBG1,"Downloading %s %s %s",name.c_str(),version.c_str(),type.c_str());
    // loop through REPOS
    downloadRepo(format("base/src/%s.%s.spm.tar.gz",name.c_str(),type.c_str()), packagePath);

    
}

/*
Some explanation about the ALL_FILE
Its a file where all packages names are stored
It is a json file
it's organized like this :

{
    "all" :
    [
        {
            "name" : "package1",
            "type" : "bin",
            "version" : "1.0.0",

        },
        {
            "name" : "package2",
            "type" : "src",
            "version" : "1.0.0",
        } 
    ]
}

*/
