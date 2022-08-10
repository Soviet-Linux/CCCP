#include <curl/curl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


 // class stuff
#include "../../include/libspm.h"
#include "../../include/globals.h"
#include "../../include/utils.h"
#include "../../include/data.h"

int get(char* p_name,char* out_path)
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
            return -1;
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

    struct package i_pkg;
    
    int r = find_data(ALL_FILE,&i_pkg);


    msg(DBG1,"Downloading %s %s %s",i_pkg.name,i_pkg.version,i_pkg.type);
    // loop through REPOS
    downloadRepo(format("base/%s/%s.%s.spm.tar.gz",typestr(i_pkg.type),i_pkg.name,i_pkg.type), out_path);
 
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
