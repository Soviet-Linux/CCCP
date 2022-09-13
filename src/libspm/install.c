#define _GNU_SOURCE


// class stuff
#include "../../include/libspm.h"
#include "../../include/utils.h"
#include "../../include/spm.h"
#include "../../include/data.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/*
In this function we are installing source or binary packages.
Source packages are archive files containing the source code of the package ,the post install script and an .spm file for the install commands.
Binary packages are archive files containing the compiled binary files of the package , the post install script and an .spm file for the install commands.
*/


// parsing data and installing package archive (with sources)

int installSpmFile(char* spm_path,int as_dep)
{
    struct package pkg;

    open_spm(spm_path, &pkg);

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
    char* package_dir = format("%s/%s-%s",MAKE_DIR,pkg.name,pkg.version);

    // making the package
    msg(DBG1,"Making %s",pkg.name);
    make(package_dir,&pkg);
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
   

    create_spm(format("%s/%s.spm",SPM_DIR,pkg.name),&pkg);

    store_data(INSTALLED_DB,&pkg ,as_dep);

    // now we need to clean everything 
    clean();





    return 0;
}

int installSpmBinary(char* archivePath,int as_dep)
{
    struct package pkg;

    pkg.name = get_bin_name(archivePath);
    if (pkg.name == NULL)
    {
        msg(ERROR,"Could not get name from archive path");
        return -1;
    }

    //uncompressing binary and checking output
    if (uncompress_binary(archivePath,BUILD_DIR) != 0) return -1;

    
    char* spm_path = format("%s/%s.spm",SPM_DIR,pkg.name);
    if (access(spm_path,F_OK) != 0)
    {
        msg(ERROR,"%s not found",spm_path);
        return -1;
    }


    open_spm(spm_path,&pkg);

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

    create_spm(format("%s/%s.spm",SPM_DIR,pkg.name),&pkg);

    store_data(format("%s/%s.spm",SPM_DIR,pkg.name),&pkg ,as_dep);

    // now we need to clean everything 
    clean();


    return 0;
}

int uncompress_binary(char* bin_path,char* dest_dir)
{
    char* untar_cmd = format("tar -xf %s -C %s",bin_path,dest_dir);

    return system(untar_cmd);
}
char* get_bin_name(char* bin_path)
{
    char* file_name = basename(bin_path);
    for (int i = 0; i < strlen(file_name); i++)
    {
        if (file_name[i] == '.')
        {
            return format("%.*s",file_name,i);
        }
    }
    return NULL;
}
bool is_installed(char* name)
{
    char* spm_path = format("%s/%s.spm",SPM_DIR,name);
    if (access(spm_path,F_OK) == 0) return true;
    return false;
}
















