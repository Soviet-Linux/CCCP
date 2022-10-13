#include "unistd.h"
#include "stdio.h"
#include "string.h"

#include "../../include/ecmp/ecmp.h"
#include "../../include/utils.h"
#include "../../include/spm/spm.h"
#include "../../include/libspm.h"


int open_pkg(char* path, struct package* pkg,char* format)
{
    msg(DBG2,"Setting everything to NULL"); 
    //set all varibales t NULL
    pkg->name = NULL;
    pkg->type = NULL;
    pkg->version = NULL;
    pkg->license = NULL;
    pkg->dependencies = NULL;
    pkg->dependenciesCount = 0;
    pkg->makedependencies = NULL;
    pkg->makedependenciesCount = 0;
    pkg->optionaldependencies = NULL;
    pkg->optionaldependenciesCount = 0;
    pkg->url = NULL;
    pkg->locations = NULL;
    pkg->locationsCount = 0;
    pkg->info.download = NULL;
    pkg->info.prepare = NULL;
    pkg->info.make = NULL;
    pkg->info.test = NULL;
    pkg->info.install = NULL;
    pkg->info.special = NULL;
    pkg->info.download = NULL;
    pkg->info.prepare = NULL;


    // print make dependencies count
    msg(DBG3,"make dependencies count : %d",pkg->makedependenciesCount);

    // check if file exists
    if (access(path,F_OK) != 0)
    {
        printf("File %s does not exist\n",path);
        return 1;
    }
    //check file extension


    /* This illustrates strrchr */

    if (format == NULL)
    {
        format = strrchr( path, '.' );
    }  

    if (format != NULL)
    {
        if (strcmp(format,".ecmp") == 0)
        {
            msg(INFO,"File %s is an ecmp file",path);
            return open_ecmp(path,pkg);
        }
        else if (strcmp(format,".spm") == 0)
        {
            msg(INFO,"File %s is an spm file",path);
            return open_spm(path,pkg);
        }
        else
        {
            msg(ERROR,"File %s is not a valid package file",path);
            return 1;
        }
    }
    else
    {
        msg(ERROR,"File %s is not a valid package file",path);
        return 1;
    }

}

int create_pkg(char* path,struct package* pkg,char* format)
{
    msg(INFO,"Creating package %s",path);

    int type;

    // get file extension
    if (format == NULL)
    {
        format = strrchr( path, '.' );
    } 
    /* This illustrates strrchr */
    if (format != NULL)
    {
        if (strcmp(format,".ecmp") == 0)
        {
            msg(WARNING,"ECMP format is still experimental");
            return create_ecmp(path,pkg);
        }
        else if (strcmp(format,".spm") == 0)
        {
            msg(INFO,"File %s is an spm file",path);
            return create_spm(path,pkg);
        }
        else
        {
            msg(ERROR,"Invalid package type");
            return 1;
        }
    }
    return -1;
}