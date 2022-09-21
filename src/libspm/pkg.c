#include "unistd.h"
#include "stdio.h"
#include "string.h"

#include "../../include/ecmp/ecmp.h"
#include "../../include/utils.h"
#include "../../include/spm/spm.h"

int open_pkg(char* path, struct package* pkg)
{
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
        return create_spm(path,pkg);
    }
    else if (type == ECMP)
    {
        return create_ecmp(path,pkg);
    }
    else
    {
        msg(ERROR,"Invalid package type");
        return 1;
    }
}