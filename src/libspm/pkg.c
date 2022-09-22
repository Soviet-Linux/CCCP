#include "unistd.h"
#include "stdio.h"
#include "string.h"

#include "../../include/ecmp/ecmp.h"
#include "../../include/utils.h"
#include "../../include/spm/spm.h"
#include "../../include/libspm.h"


int open_pkg(char* path, struct package* pkg)
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

    // check if file exists
    if (access(path,F_OK) != 0)
    {
        printf("File %s does not exist\n",path);
        return 1;
    }
    //check file extension

    // get file extension
    char * ptr;
    /* This illustrates strrchr */
    ptr = strrchr( path, '.' );
    if (ptr != NULL)
    {
        if (strcmp(ptr,".ecmp") == 0)
        {
            msg(INFO,"File %s is an ecmp file",path);
            return open_ecmp(path,pkg);
        }
        else if (strcmp(ptr,".spm") == 0)
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

int create_pkg(char* path,struct package* pkg,int type)
{
    msg(INFO,"Creating package %s",path);
    if (type == 0)
    {
        type = DEFAULT_FORMAT;
    }

    if (type == SPM)
    {
        msg(INFO,"Creating spm package");
        return create_spm(path,pkg);
    }
    else if (type == ECMP)
    {
        msg(WARNING,"ECMP format is still experimental");
        return create_ecmp(path,pkg);
    }
    else
    {
        msg(ERROR,"Invalid package type");
        return 1;
    }
}