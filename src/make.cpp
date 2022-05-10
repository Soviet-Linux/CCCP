#include <string>
#include <vector>
#include <iostream>

//class stuff
#include "../include/cccp.hpp"

/*
    All hte complexity in this function and really in this entire project if just because we need to track  files installed by a makefile
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
void soviet::package::make ()
{
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

    // this solution with the '*' is very bad i think
    std::string package_dir = MAKE_DIR + name + "*";

    /*
        So in this part we are foramtting and executing the commands to configure , compile , test and install the package.
        The test command is executed if TESTING is set to true
        The "BUILD_ROOT" part is pass the BUILD_DIR path (Where we will be installing the packages) to the command.
        It is ment to be used in the install or configure command : make prefix=$BUILD_ROOT install or ./configure --prefix=$BUILD_ROOT or smth like that.
    */
    // TODO: find someone intelligent to ameliorate this code

    //checking is the command are used and formatting and executing them
    if (!prepare_info.empty())
    {
        //formatting the prepare command
        std::string prepare_cmd = "cd " + package_dir + " && " + prepare_info;
        //Printing the command to the terminal
        std::cout << prepare_cmd << std::endl;
        //executing the command
        system(prepare_cmd.c_str());
    }
    if (!build_info.empty())
    {
        //Formating the command
        std::string make_cmd = "BUILD_ROOT="+ BUILD_DIR +"; ( cd " + package_dir + " && "  + build_info + " )";
        // printing the command to standard output 
        std::cout << make_cmd << std::endl;
        //executing the command
        system(make_cmd.c_str());
        
    }

    // executing the package test suite if TESTING is set to true and storing the tests results in the LOG_DIR
    if (TESTING && !test_info.empty()) system(("( cd "+ package_dir + " && " + test_info + " > "+ LOG_DIR + name + ".test )").c_str());

    //installing the package in the build directory

    //formatting the install command
    std::string install_cmd = "BUILD_ROOT="+ BUILD_DIR +"; ( cd "+ package_dir + " && " + install_info + " )";

    //printing , for debugging purposes
    std::cout << install_cmd << std::endl; 

    //And finally , executing the install command
    system(install_cmd.c_str());

    // i have a small problem here , i dont know what to do with the package spm file so im just gonna move it in a random place 
    // and then move it to the correct location
    rename(soviet::format("%s/%s.spm",MAKE_DIR.c_str(),name.c_str()) , soviet::format("/tmp/%s.tmp.spm",name.c_str()));

    // cleaning up the build directory
    system(("rm -rf " + package_dir).c_str());
}
