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



char* get(struct package *i_pkg,char* out_path)
{
    // I commented this part because the soviet system im working on right now doesnt support curl 
    // I will add it later whan the rest of the stuff is ready 

    // check if ALL_FILE exists
    if (i_pkg->name == NULL)
    {
        msg(ERROR,"Package name not specified!");
        return NULL;
    }

    if (access((ALL_DB),F_OK)!=0)
    {
        msg(ERROR,"Global package data file not found, to download it use -s option!");
        return NULL;
    }
    msg(DBG1,"Loading %s\n",  ALL_DB);
    

    char* pkg_format = calloc(64,sizeof(char));
    

    find_data_repo(ALL_DB,i_pkg,&pkg_format);
    printf("format is %s\n",pkg_format);

    // we need to add a way to check if the repo that we are using is on the same version as the one in the database
    // if not , we need to update the database*


    msg(DBG1,"Downloading %s %s %s",i_pkg->name,i_pkg->version,i_pkg->type);

    // loop through REPOS
    char* url = calloc(64+strlen(i_pkg->type)+strlen(i_pkg->name)+strlen(pkg_format),sizeof(char));
    sprintf(url,"base/%s/%s.%s",i_pkg->type,i_pkg->name,pkg_format);

    if (downloadRepo(url, out_path) != 0)
    {
        msg(ERROR,"Failed to download %s",url);
        return NULL;
    } 
    free(url);

    return pkg_format;
}


