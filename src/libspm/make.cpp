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
        std::string prepare_cmd = soviet::format("BUILD_ROOT=%s; ( cd %s && %s )",BUILD_DIR.c_str(),package_dir.c_str(),prepare_info.c_str());

        //Printing the command to the terminal
        if (DEBUG) std::cout << prepare_cmd << std::endl;
        //executing the command
        // We add the extra command parameters to the command , so that the user can add extra parameters to the command
        exec(prepare_cmd.c_str());
        //debug
        std::cout << "prepare command executed" << std::endl;
    }
    if (!build_info.empty())
    {
        //Formating the command
        std::string make_cmd = soviet::format("BUILD_ROOT=%s; ( cd %s && %s )",BUILD_DIR.c_str(),package_dir.c_str(),build_info.c_str());
        // printing the command to standard output 
        if (DEBUG) std::cout << make_cmd << std::endl;
        //executing the command
        exec(make_cmd.c_str());
        //debug
        if (DEBUG) std::cout << "build done" << std::endl;

        
    }

    // executing the package test suite if TESTING is set to true and storing the tests results in the LOG_DIR
    if (soviet::TESTING && !test_info.empty()) 
    {
        std::string test_result  = exec(format("( cd %s  &&  %s )",package_dir.c_str(),test_info.c_str()));
        std::ofstream log_file;
        log_file.open(format("%s/%s.test",LOG_DIR.c_str(),name.c_str()));
        log_file << test_result << std::endl;

    }
    

    //installing the package in the build directory

    //formatting the install command
    std::string install_cmd = soviet::format("BUILD_ROOT=%s ; ( cd %s && %s )",soviet::BUILD_DIR.c_str(),package_dir.c_str(),install_info.c_str());

    //printing , for debugging purposes
    if (DEBUG) std::cout << install_cmd << std::endl; 

    //And finally , executing the install command
    exec(install_cmd.c_str());

    //debug
    if (DEBUG) std::cout << "install done" << std::endl;
    
    /*
    //moving temporary spm files to build dir to match bin package look
    if (DEBUG) std::cout << rename(format("%s/%s.spm",MAKE_DIR.c_str(),name.c_str()),format("%s/%s.spm",BUILD_DIR.c_str(),name.c_str())) << "\n";
    if (soviet::DEBUG) std::cout << "Spm file moved from " << format("%s/%s.spm",MAKE_DIR.c_str(),name.c_str()) << " to " << format("%s/%s.spm",BUILD_DIR.c_str(),name.c_str()) << "\n";
    */
    // WOOW I'm Happy Right Now , it finally got rid af this annying code block above.
    // I left it as a comment because people need to know what bad code is
}