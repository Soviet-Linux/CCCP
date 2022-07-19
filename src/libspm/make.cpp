#include <fstream>
#include <string>
#include <vector>
#include <iostream>
//I hate this , and i dont know why , but im forced to use  it because there is no other good alternative
#include <filesystem>

//class stuff
#include "../../include/libspm.hpp"

/*
    All the complexity in this function and really in this entire project if just because we need to track  files installed by a makefile
    For now we juste install in a separate directory (BUILD_DIR) and then move it to the correct location (The real filesystem)
    I tried many solutions to do this like installwatch , checkinstall , but none of them worked for me.
    If you have an idea PLEASE , I beg you tell me or write it directly here .
    IDEAS :
    - 
    -
    -
    I prefer simple solution that deosnt include to write a timestamp based install-log thing from scratch
    (I tried , but its not good enough)

*/
int soviet::package::make (const std::string& package_dir)
{
    std::string cmd_params = "";
    
    //If debug is not enabled , reidrecting all command output to /dev/null
    if (vars.QUIET) cmd_params = "&> /dev/null";
    // this is actually a great piece of code 

    
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


    /*
        So in this part we are foramtting and executing the commands to configure , compile , test and install the package.
        The test command is executed if TESTING is set to true
        The "BUILD_ROOT" part is pass the BUILD_DIR path (Where we will be installing the packages) to the command.
        It is ment to be used in the install or configure command : make prefix=$BUILD_ROOT install or ./configure --prefix=$BUILD_ROOT or smth like that.
    */
    // TODO: find someone intelligent to ameliorate this code

    //checking is the command are used and formatting and executing them
    if (!info["prepare"].empty())
    {
        //formatting the prepare command
        std::string prepare_cmd = soviet::format("BUILD_ROOT=%s; ( cd %s && %s ) ",vars.BUILD_DIR.c_str(),package_dir.c_str(),info["prepare"].c_str());

        //Printing the command to the terminal
       msg(DBG2,"Executing prepare command : %s",prepare_cmd.c_str());
        //executing the command
        // We add the extra command parameters to the command , so that the user can add extra parameters to the command
        if (system((prepare_cmd + cmd_params).c_str())) return 1;
        //debug
        msg(DBG1,"prepare command executed !");
    }
    if (!info["build"].empty())
    {
        //Formating the command
        std::string make_cmd = soviet::format("BUILD_ROOT=%s; ( cd %s && %s ) ",vars.BUILD_DIR.c_str(),package_dir.c_str(),info["build"].c_str());
        // printing the command to standard output if debug is enabled
        msg(DBG3,"executing build command : %s",make_cmd.c_str());
        //executing the command
        if (system((make_cmd + cmd_params).c_str())) return 1;
        //debug
        msg(DBG1,"Build done !");
    

        
    }


    

    //installing the package in the build directory

    //formatting the install command
    std::string install_cmd = soviet::format("BUILD_ROOT=%s ; ( cd %s && %s ) ",vars.BUILD_DIR.c_str(),package_dir.c_str(),info["install"].c_str());

    //printing , for debugging purposes
    msg(DBG3,"Executing install command : %s",install_cmd.c_str());

    //And finally , executing the install command
    if (system((install_cmd + cmd_params).c_str())) return 1;

    //debug
    msg(DBG1,"Install done !");
    
    return 0;

}