#include <curl/curl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


 // class stuff
#include "../../include/libspm.h"
#include "../../include/globals.h"
#include "../../include/utils.h"
#include "../../include/data.h"



char* get(char* p_name,char* out_path)
{
    // I commented this part because the soviet system im working on right now doesnt support curl 
    // I will add it later whan the rest of the stuff is ready 
    
    // check if ALL_FILE exists
    if (access((ALL_DB),F_OK))
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
            return NULL;
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
    msg(DBG1,"Loading %s",  ALL_DB);
    // verify if package exists
    // parse ALL_FILE and check if the package is there

    struct package i_pkg;
    
    
    find_data(ALL_DB,&i_pkg);

    // we need to add a way to check if the repo that we are using is on the same version as the one in the database
    // if not , we need to update the database*

    if (check_repo_version(get_data_version(format("%s/VERSION",DATA_DIR))) != 0)
    {
        msg(ERROR,"The local database version is not the same as the one in the repository, please update the local database!");
        return NULL;
    }

    msg(DBG1,"Downloading %s %s %s",i_pkg.name,i_pkg.version,i_pkg.type);
    // loop through REPOS
    downloadRepo(format("base/%s/%s.%s.spm.tar.gz",i_pkg.type,i_pkg.name,i_pkg.type), out_path);
 
    return i_pkg.type; 
}

int check_repo_version(int V_LOCAL)
{
    char* V_CHECK = mktemp("/tmp/V_CHECK.XXXXXX");
    downloadRepo("VERSION", V_CHECK);
    char* V_REPO; 
    rdfile(V_CHECK, &V_REPO);
    if (atoi(V_REPO) != V_LOCAL)
    {
        return 1;
    }
    return 0;
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
