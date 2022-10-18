#include "stdio.h"
#include "stdlib.h"
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


//class stuff
#include "../../include/libspm.h"
#include "../../include/utils.h"
#include "../../include/globals.h"

/*
    All the complexity in this function and really in this entire project if just because we need to track  files installed by a makefile
    For now we juste install in a separate directory (BUILD_DIR) and then move it to the correct location (The real filesystem)
    I tried many solutions to do this like installwatch , checkinstall , but none of them worked for me.
    If you have an idea PLEASE , I beg you tell me or write it directly here .
    IDEAS :
    - 
    -
    -
    I prefer simple solution that doesnt include to write a timestamp based install-log thing from scratch
    (I tried , but its not good enough)

*/
int make (char* package_dir,struct package* pkg)
{

    char *cmd_params;
    
    //If debug is not enabled , reidrecting all command output to /dev/null
    if (QUIET) {
        cmd_params = "&> /dev/null";
    }
    else {
        cmd_params = "";
    }
    

    /*
        We have some problems here , because some complex packages require advanced options to be installed 
        (like glibc that wants a separate build dir )
        For nom i resolved that by adding more complexity to the build commands , but its not pretty.
        So i dont know i we should implement these options directly here and drop a lot of performance juste for a small number of packages 
        or if we leave like that with some ugly command in the .spm file
        If you come across this you can vote :

        change -> 0
        keep like that -> 1

        Also this code is very bad so if you dont understand it dm me on discord : PKD667#9879 .
    */
    //TODO: resolve this issue (the original TODO was "fix this shit" , but i think "resolve this issue is better")

    printf("prepare : %s\n",pkg->info.prepare);

   



    /*
        So in this part we are foramtting and executing the commands to configure , compile , test and install the package.
        The test command is executed if TESTING is set to true
        The "BUILD_ROOT" part is pass the BUILD_DIR path (Where we will be installing the packages) to the command.
        It is ment to be used in the install or configure command : make prefix=$BUILD_ROOT install or ./configure --prefix=$BUILD_ROOT or smth like that.
    */
    // TODO: find someone intelligent to ameliorate this code

    // Idk why i havent done this before , but i moved the downloading here
    if (pkg->info.download != NULL && strlen(pkg->info.download) > 0) {
        char* sources_cmd = calloc(
            64 + strlen(pkg->name) + strlen(pkg->version) + strlen(pkg->url) + 
            strlen(MAKE_DIR) + strlen(pkg->info.download), 
            sizeof(char));

        sprintf(sources_cmd,
            " NAME=%s && VERSION=%s && URL=%s && cd %s && %s",
            pkg->name,pkg->version,pkg->url,MAKE_DIR,pkg->info.download);

        msg(DBG2,"Downloading sources with %s",sources_cmd);
        if (system(sources_cmd) != 0) {
            msg(ERROR,"Failed to download sources for %s",pkg->name);
            free(sources_cmd);
            return -1;
        }
        free(sources_cmd);
    }
    //checking is the command are used and formatting and executing them
    if (pkg->info.prepare != NULL && strlen(pkg->info.prepare) > 0) 
    {
        //formatting the prepare command
        char* prepare_cmd = calloc(
            64 + strlen(BUILD_DIR) + strlen(package_dir) + 
            strlen(pkg->info.prepare) + strlen(cmd_params), 
            sizeof(char));

        sprintf(prepare_cmd,
            "BUILD_ROOT=%s; ( cd %s && %s ) %s",
            BUILD_DIR,package_dir,pkg->info.prepare,cmd_params);

        //Printing the command to the terminal
        msg(DBG2,"Executing prepare command : %s",prepare_cmd);
        //executing the command
        // We add the extra command parameters to the command , so that the user can add extra parameters to the command
        if (system(prepare_cmd) != 0) {
            free(prepare_cmd);
            return 1;
        }
        //debug
        msg(DBG1,"prepare command executed !");

        free(prepare_cmd);
    }
    msg(DBG3,"Make command is %s",pkg->info.make);
    if (pkg->info.make != NULL && strlen(pkg->info.make) > 0) 
    {
        //formatting the prepare command
        char* make_cmd = calloc(
            64 + strlen(BUILD_DIR) + strlen(package_dir) + 
            strlen(pkg->info.make) + strlen(cmd_params), 
            sizeof(char));

        sprintf(make_cmd,
            "BUILD_ROOT=%s; ( cd %s && %s ) %s",
            BUILD_DIR,package_dir,pkg->info.make,cmd_params);

        //Printing the command to the terminal
        msg(DBG2,"Executing make command : %s",make_cmd);
        //executing the command
        // We add the extra command parameters to the command , so that the user can add extra parameters to the command
        if (system(make_cmd) != 0) {
            free(make_cmd);
            return 1;
        }
        //debug
        msg(DBG1,"make command executed !");

        free(make_cmd);
    

        
    }
    if (pkg->info.test != NULL && TESTING && strlen(pkg->info.test) > 0) 
    {
        //formatting the  command
        char* test_cmd = calloc(
            64 + strlen(BUILD_DIR) + strlen(package_dir) + 
            strlen(pkg->info.test) + strlen(cmd_params), 
            sizeof(char));

        sprintf(test_cmd,
            "BUILD_ROOT=%s; ( cd %s && %s ) %s",
            BUILD_DIR,package_dir,pkg->info.test,cmd_params);


        //Printing the command to the terminal
        msg(DBG2,"Executing test command : %s",test_cmd);
        //executing the command
        // We add the extra command parameters to the command , so that the user can add extra parameters to the command
        if (system(test_cmd) != 0) {
            free(test_cmd);
            return 1;
        };
        //debug
        msg(DBG1,"make command executed !");

        free(test_cmd);
    }
    printf("install : %s\n",pkg->info.install);
    if (pkg->info.install != NULL && strlen(pkg->info.install) > 0) 
    {
        //formatting the prepare command
        char* install_cmd = calloc(
            64 + strlen(BUILD_DIR) + strlen(package_dir) + 
            strlen(pkg->info.install) + strlen(cmd_params), 
            sizeof(char));

        sprintf(install_cmd,
            "BUILD_ROOT=%s; ( cd %s && %s ) %s",
            BUILD_DIR,package_dir,pkg->info.install,cmd_params);

        //Printing the command to the terminal
        msg(DBG2,"Executing Install command : %s",install_cmd);
        //executing the command
        // We add the extra command parameters to the command , so that the user can add extra parameters to the command
        if (system(install_cmd) != 0) 
        {
            free(install_cmd);
            msg(FATAL,"Failed to install %s",pkg->name);
            return -2;
        }
        //debug
        msg(DBG1,"install command executed !");

        free(install_cmd);
    }
    else {
        msg(FATAL,"No install command !");
    }
    
    return 0;

}
int exec_special(char* cmd,char* package_dir)
{
    msg(DBG2,"Executing special command : %s",cmd);
    char* special_cmd = calloc(
        64 +strlen(package_dir) + strlen(cmd), 
        sizeof(char));

    if (system(special_cmd) != 0) {
        free(special_cmd);
        return 1;
    }
    free(special_cmd);
    msg(DBG1,"special command executed !");
    return 0;
}