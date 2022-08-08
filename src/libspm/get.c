#include <curl/curl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
//include json lib


 // class stuff
#include "../../include/libspm.h"
#include "../../include/globals.h"

void get()
{
    // I commented this part because the soviet system im working on right now doesnt support curl 
    // I will add it later whan the rest of the stuff is ready 
    
    // check if ALL_FILE exists
    if (access((ALL_FILE),F_OK))
    {
        // This is the first tim is use a ''' do {...} while(...) ''' loop
        char input;
        do
        {
            printf("There are no package data file , do you want to download it ? [y/n]");
            
            scanf("%c",&input);
            
        }
        while(  input != 'y' && input != 'n');
        if (input == 'n')
        {
            msg(ERROR, "No package data file found, to download it use -s option!");
            return;
        }  
        else if (input == 'y')
        {
            sync();
        } 
        else 
        {
            msg(FATAL,"Invalid input");
        }

        
    }
    msg(DBG1,"Loading %s",  ALL_FILE);
    // verify if package exists
    // parse ALL_FILE and check if the package is there
    std::ifstream file_spm((ALL_FILE), std::ios::in);
    std::stringstream buffer;
    buffer << file_spm.rdbuf();
    //parsing json data
    auto all_pkgs = json::parse(buffer.str());
    msg(DBG1,"Parsing %s" ,ALL_FILE);
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
        msg(FATAL,"Package %s not found",name);
        return;
    }
    else {
        msg(INFO,"Package %s %s %s found",name,version,type);
    }
    // Defining package path , the location where the packages will be downloaded
    packagePath = soviet::format("%s/%s.%s.spm.tar.gz",TMP_DIR, name, type);
    

    msg(DBG1,"Downloading %s %s %s",name,version,type);
    // loop through REPOS
    downloadRepo(format("base/src/%s.%s.spm.tar.gz",name,type), packagePath);

    
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
