#include "unistd.h"
#include "stdio.h"
#include "string.h"

#include "../../include/utils.h"
#include "../../include/libspm.h"
#include <dlfcn.h>
#include <stdlib.h>


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
        msg(DBG2,"Getting format from file extension");
        format = strrchr( path, '.' ) + 1;
        printf("Format : %s\n",format);
    }  
    
    if (format != NULL)
    {
        // this is experimental
        for (int i = 0; i < FORMAT_COUNT; i++)
        {
            printf("format : %s = %s\n",format,FORMATS[i]);
            if (strcmp(format,FORMATS[i]) == 0)
            {
                msg(DBG2,"Opening package with %s format",FORMATS[i]);
                runFormatLib(FORMATS[i],"open",path,pkg);
                return 0;
            }
        }
    }
    else
    {
        msg(ERROR,"File %s is not a valid package file",path);
        return 1;
    }
    msg(ERROR,"File %s is not a valid package file, or the format plugin isn't loaded",path);
    return 1;

}

int create_pkg(char* path,struct package* pkg,char* format)
{
    msg(INFO,"Creating package %s",path);

    int type;

    // get file extension
    if (format == NULL)
    {
        format = strrchr( path, '.' ) + 1;
    } 
    /* This illustrates strrchr */
    if (format != NULL)
    {
        // this is experimental
        for (int i = 0; i < FORMAT_COUNT; i++)
        {
            if (strcmp(format,FORMATS[i]) == 0)
            {
                msg(DBG2,"Opening package with %s format",FORMATS[i]);
                runFormatLib(FORMATS[i],"create",path,pkg);
                return 0;
            }
        }
    }
    msg(ERROR,"File %s is not a valid package file, or the format plugin isn't loaded",path);
    return -1;
}

int runFormatLib (char* format,char* fn,char* pkg_path,struct package* pkg)
{
    char* lib_path = calloc(64,sizeof(char));
    sprintf(lib_path,"%s/%s.so",PLUGIN_DIR,format);
    msg(DBG2,"Loading %s",lib_path);

    if (access(lib_path,F_OK) != 0)
    {
        msg(ERROR,"File %s does not exist",lib_path);
        return 1;
    }
    
    // load fn from so lib
    void* handle = dlopen(lib_path,RTLD_LAZY);
    if (!handle)
    {
        fprintf(stderr,"%s\n",dlerror());
        return 1;
    }
    int (*func)(char*,struct package*) = dlsym(handle,fn);
    char* error = dlerror();
    if (error != NULL)
    {
        fprintf(stderr,"%s\n",error);
        return 1;
    }
    if (func(pkg_path,pkg) != 0)
    {
        return -1;
    }

    dlclose(handle);
    return 0;

}