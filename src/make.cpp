#include <string>
#include <vector>
#include <iostream>
//I hate this , and i dont know why , but im forced to use  it because there is no other good alternative
#include <filesystem>

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

    std::string package_dir = soviet::format("%s/%s-%s", soviet::MAKE_DIR.c_str(), name.c_str(), version.c_str());

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
        std::string prepare_cmd = soviet::format("( cd %s && %s )",package_dir.c_str(),prepare_info.c_str());

        //Printing the command to the terminal
        std::cout << prepare_cmd << std::endl;
        //executing the command
        system(prepare_cmd.c_str());
    }
    if (!build_info.empty())
    {
        //Formating the command
        std::string make_cmd = soviet::format("( cd %s && %s )",package_dir.c_str(),build_info.c_str());
        // printing the command to standard output 
        std::cout << make_cmd << std::endl;
        //executing the command
        system(make_cmd.c_str());
        
    }

    // executing the package test suite if TESTING is set to true and storing the tests results in the LOG_DIR
    if (soviet::TESTING && !test_info.empty()) system(("( cd "+ package_dir + " && " + test_info + " > "+ LOG_DIR + name + ".test )").c_str());

    //installing the package in the build directory

    //formatting the install command
    std::string install_cmd = soviet::format("BUILD_ROOT=%s ; ( cd %s && %s )",soviet::BUILD_DIR.c_str(),package_dir.c_str(),install_info.c_str());

    //printing , for debugging purposes
    std::cout << install_cmd << std::endl; 

    //And finally , executing the install command
    system(install_cmd.c_str());

    // i have a small problem here , i dont know what to do with the package spm file so im just gonna move it in a random place 
    // and then move it to the correct location
    rename(soviet::format("%s/%s.spm",MAKE_DIR.c_str(),name.c_str()) , soviet::format("/tmp/%s.spm.tmp",name.c_str()));

    // cleaning up the build directory
    // You may have noticed that i HATE the std::filesystem thing 
    // But some people on discord told me that system(rm -rf) is bad
    // They are right 
    // But still , i think std::filesystem is worse
    // too many "::" , its annoying 
    // also my clangd server doesnt recognize the std::filesystem
    // So my IDE is telling me that its an error , but its not 
    // It's very annoying to me
    // But you don't care
    // So its all good
    // I'm very close to write an entire c function to replace this shit
    // But i dont have time to do that
    std::filesystem::remove_all(package_dir)
}
