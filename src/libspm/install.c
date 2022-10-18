#include <stdio.h>
#define _GNU_SOURCE


// class stuff
#include "../../include/libspm.h"
#include "../../include/utils.h"
#include "../../include/data.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>


/*
In this function we are installing source or binary packages.
Source packages are archive files containing the source code of the package ,the post install script and an .spm file for the install commands.
Binary packages are archive files containing the compiled binary files of the package , the post install script and an .spm file for the install commands.
*/


// parsing data and installing package archive (with sources)

int installSpmFile(char* spm_path,int as_dep)
{
    struct package pkg;

    open_pkg(spm_path, &pkg,NULL);

    PACKAGE_QUEUE[QUEUE_COUNT] = pkg.name; // add this shit to the PKG_QUEUE ARRAY
    QUEUE_COUNT++;
    msg(DBG1,"Added %s to the queue",pkg.name);

    /*
        the following code is pretty bad.
        I'll let i like that for now wince its needed
        but I'll try to improve it later

        TODO: Fix this shit
    */



    // check if package is already installed
    if (is_installed(pkg.name))
    {
        msg(DBG1,"Package %s is already installed, reinstalling",pkg.name);
        uninstall(pkg.name);
    }

    if (pkg.dependencies != NULL && pkg.dependenciesCount > 0 && strlen(pkg.dependencies[0]) > 0)

    {
        msg(DBG1,"Checking dependencies...");
        check_dependencies(pkg.dependencies,pkg.dependenciesCount);
    }
    // checking makedeps
    if (pkg.makedependencies != NULL && pkg.makedependenciesCount > 0 && strlen(pkg.makedependencies[0]) > 0)
    {
        msg(DBG3,"Checking makedeps : %s",pkg.makedependencies);
        check_dependencies(pkg.makedependencies,pkg.makedependenciesCount);

    }


    /* 
        here we have some problems:
            The legacy package dir was in MAKE_DIR/$NAME-$VERSION
        Should we keep it or not ? 
        I choose for compatibility reasons to keep it.
        If soeone wants to chnage this you can vote here :
         - keep it : 1
         - chnage it  : 0
    */

    char* legacy_dir = malloc(strlen(MAKE_DIR)+strlen(pkg.name)+strlen(pkg.version)+2);
    sprintf(legacy_dir,"%s/%s-%s",MAKE_DIR,pkg.name,pkg.version);


    // making the package
    msg(DBG1,"Making %s",pkg.name);
    make(legacy_dir,&pkg);
    msg(DBG1,"Making %s done",pkg.name);


    // getting locations
    msg(DBG1,"Getting locations for %s",pkg.name);
    pkg.locationsCount = get_locations(&pkg.locations,BUILD_DIR);
    
    // moving binaries
    msg(DBG1,"Moving binaries for %s",pkg.name);
    move_binaries(pkg.locations,pkg.locationsCount);
    msg(DBG1,"Moving binaries for %s done",pkg.name);

    

    //  executing post install scripts
    // check if pkg.info.special is not empty or NULL
    if (pkg.info.special != NULL && strlen(pkg.info.special) > 0)
    {
        msg(DBG1,"Executing post install script for %s",pkg.name);
        exec_special(pkg.info.special,BUILD_DIR);
        msg(DBG1,"Executing post install script for %s done",pkg.name);
    }
   
    // remove the deprecated unsafe format function call
    // format the path using sprintf
    char* file_path = calloc(strlen(SPM_DIR)+strlen(pkg.name)+strlen(pkg.version)+2,sizeof(char));
    sprintf(file_path,"%s/%s-%s",SPM_DIR,pkg.name,pkg.version);
    create_pkg(file_path,&pkg,NULL);
    free(file_path);

    store_data(INSTALLED_DB,&pkg ,as_dep);

    // now we need to clean everything 
    clean();





    return 0;
}

int installSpmBinary(char* archivePath,int as_dep)
{
    struct package pkg;

    pkg.name = calloc(sizeof(archivePath),sizeof(char));
    
    if (get_bin_name(archivePath,pkg.name) != 0)
    {
        msg(ERROR,"Could not get name from archive path");
        return -1;
    }

    //uncompressing binary and checking output
    if (uncompress_binary(archivePath,BUILD_DIR) != 0) return -1;

    // format the path using sprintf
    char* spm_path = calloc(strlen(BUILD_DIR)+strlen(pkg.name)+strlen(DEFAULT_FORMAT)+2,sizeof(char));
    sprintf(spm_path,"%s/%s.%s",BUILD_DIR,pkg.name,DEFAULT_FORMAT);
    if (access(spm_path,F_OK) != 0)
    {
        msg(ERROR,"%s not found",spm_path);
        return -1;
    }


    open_pkg(spm_path,&pkg,NULL);

    PACKAGE_QUEUE[QUEUE_COUNT] = pkg.name; // add this shit to the PKG_QUEUE ARRAY
    QUEUE_COUNT++;
    msg(DBG1,"Added %s to the queue",pkg.name);

    check_dependencies(pkg.dependencies,pkg.dependenciesCount);

        // moving binaries
    msg(DBG1,"Moving binaries for %s",pkg.name);
    move_binaries(pkg.locations,pkg.locationsCount);
    msg(DBG1,"Moving binaries for %s done",pkg.name);

    

    //  executing post install scripts
    exec_special(pkg.info.special,BUILD_DIR);

    // format the path using sprintf
    char* create_path = calloc(strlen(SPM_DIR)+strlen(pkg.name)+strlen(DEFAULT_FORMAT)+2,sizeof(char));
    sprintf(create_path,"%s/%s.%s",SPM_DIR,pkg.name,DEFAULT_FORMAT);
    create_pkg(create_path,&pkg,NULL);

    // format
    char* store_path = calloc(strlen(SPM_DIR)+strlen(pkg.name)+strlen(DEFAULT_FORMAT)+2,sizeof(char));
    sprintf(store_path,"%s/%s.%s",SPM_DIR,pkg.name,DEFAULT_FORMAT);
    store_data(store_path,&pkg ,as_dep);

    // now we need to clean everything 
    clean();


    free(spm_path);
    free(create_path);
    free(store_path);

    free_pkg(&pkg);

    return 0;
}

int uncompress_binary(char* bin_path,char* dest_dir)
{
    // format the path using sprintf
    char* untar_cmd = calloc(strlen(bin_path)+strlen(dest_dir)+64,sizeof(char));
    sprintf(untar_cmd,"tar -xvf %s -C %s",bin_path,dest_dir);

    int EXIT = system(untar_cmd);

    free(untar_cmd);
    return EXIT;
}
int get_bin_name(char* bin_path,char* name)
{
    char* file_name = basename(bin_path);
    for (int i = 0; i < strlen(file_name); i++)
    {
        if (file_name[i] == '.')
        {

            sprintf(name,"%.*s",i,file_name);
            return 0;
        }
    }
    return -1;
}
bool is_installed(char* name)
{
    char* path = calloc(strlen(SPM_DIR)+strlen(name)+128,sizeof(char));
    // loop through all formats
    for (int i = 0; i < FORMAT_COUNT; i++)
    {
        sprintf(path,"%s/%s.%s",SPM_DIR,name,FORMATS[i]);
        if (access(path,F_OK) == 0)
        {
            free(path);
            return true;
        }
    }
    free(path);
    return false;
}
















