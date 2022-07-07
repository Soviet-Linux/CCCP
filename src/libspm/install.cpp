#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <filesystem>
#include <sys/stat.h>


// class stuff
#include "../../include/libspm.hpp"


/*
In this file we are installing source or binary packages.
Source packages are archive files containing the source code of the package ,the post install script and an .spm file for the install commands.
Binary packages are archive files containing the compiled binary files of the package , the post install script and an .spm file for the install commands.
*/


// parsing data and installing package
void soviet::package::installFile()
{
    if (type == "src")
    {
        /* All these variables are bad.*/
        //location spm file in build dir
        std::string spm_build = soviet::format("%s/%s.spm",vars.BUILD_DIR.c_str(),name.c_str());


        // this access function is weird , the return is 0 if it works and -1 if it doesnt 
        if (!access(dataSpmPath.c_str(), F_OK))
        {
            msg(WARNING, "Package is already installed, reinstalling...");

            uninstall();
        }
        else
        {
            msg(DBG1,"Package %s is not installed , installing...",name.c_str());
        }

        //comand to  uncompress the .src;spm.tar.gz archive to MAKE_DIR
        std::string cmd_uncompress = soviet::format("tar -xf %s -C %s ",packagePath.c_str(),vars.MAKE_DIR.c_str());
        // if debug is on , print the command
        msg(DBG3,"Uncompressing package with %s",cmd_uncompress.c_str());
        //uncompressing <PName>.src.spm.tar.gz in PKG_DIR
        system(cmd_uncompress.c_str());

        //debug
        msg(DBG3, "%s/%s.spm",vars.MAKE_DIR.c_str(),name.c_str());
        // Reading spm file in MAKE DIR
        var_spm(soviet::format("%s/%s.spm",vars.MAKE_DIR.c_str(),name.c_str()));
        
        // Checking dependencies
        //This dependencies if;else system is deprecated and will be removed in the future
        if (check_dependencies())
        {
            std::cout << "Dependency check passed!"<< "\n";     
        }
        else
        {
            std::cout << "Dependency check failed!"<< "\n";
            // TODO: ADD THE DEPENDENCIES STUFF HERE
            exit(1);
        }

        // building the package and getting the locations
        //making package
        if (make())
        {
            //an error as occured
            std::cout << "an error has occured ! its bad , do something !" << std::endl;
            exit(1);
        }

        // I want to make a fancy auto-dection of dependencies using ldd
        // TODO: Do this later
        //
        
        // fancy output
        std::cout << "☭ Package built"<< "\n";
        //Get package locations
        get_locations();
        // Moving built binaries to their install location on the system
        move_binaries();

        //executing post installation scripts
        if (!special_info.empty()) 
        {
            system(format("(cd %s && %s ) ",vars.MAKE_DIR.c_str(),special_info.c_str()));  
        }
        else 
        {
            std::cout << "No post installation scripts found" << "\n";
        }

        // Storing package data
        // Adding the locations to the package files , and the packages files to DATA_DIR
        store_spm(soviet::format("%s/%s.spm",vars.MAKE_DIR.c_str(),name.c_str()),dataSpmPath);

        //adding the package to the data file
        add_data();

        // Cleaning everything
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
        std::filesystem::remove_all(vars.MAKE_DIR);
        std::filesystem::remove_all(vars.BUILD_DIR);
        // recreating the dir 
        mkdir(vars.BUILD_DIR.c_str(), 0777);
        mkdir(vars.MAKE_DIR.c_str(), 0777);
        /* 
        The solution i used in the code above is bad 
        If you have an idea , do it 
        The goal is to remove all the content of BUILD_DIR 
        Without using complicated stuff
        */
        

    }
    else if (type == "bin")
    {
        // checking if the package is already installed
        msg(DBG2,"Checking is %s exists",dataSpmPath.c_str());

        // this access function is weird , the return is 0 if it works and -1 if it doesnt 
        if (!access(dataSpmPath.c_str(), F_OK))
        {
            msg(WARNING, "Package is already installed, reinstalling...");
            
            // removing the old package
            uninstall();
        }
        else
        {
            msg(DBG1,"Package %s is not installed , installing...",name.c_str());
        }

        std::string cmd_uncompress = soviet::format("tar -xf %s -C %s ",packagePath.c_str(),vars.BUILD_DIR.c_str());
        // if debug is on , print the command
        msg(DBG3,"Uncompressing package with %s",cmd_uncompress.c_str());
        //uncompressing <PName>.src.spm.tar.gz in PKG_DIR
        system(cmd_uncompress.c_str());

        //debug
        msg(DBG1,"Reading %s/%s.spm",vars.BUILD_DIR.c_str(),name.c_str());
        // Reading spm file in MAKE DIR
        var_spm(soviet::format("%s/%s.spm",vars.BUILD_DIR.c_str(),name.c_str()));
        
        // Checking dependencies
        //This dependencies if;else system is deprecated and will be removed in the future
        if (check_dependencies())
        {
            std::cout << "Dependency check passed!"<< "\n";     
        }
        else
        {
            std::cout << "Dependency check failed!"<< "\n";
            // TODO: ADD THE DEPENDENCIES STUFF HERE
            exit(1);
        }
        // moving the spm file so it sint doing weird stuff
        rename(format("%s/%s.spm",vars.BUILD_DIR.c_str(),name.c_str()),format("%s/%s.spm",vars.SPM_DIR.c_str(),name.c_str()));
        // Moving built binaries to their install location on the system
        move_binaries();

        //executing post installation scripts
        if (!special_info.empty()) 
        {
            system(format("(cd %s && %s ) ",vars.BUILD_DIR.c_str(),special_info.c_str())); 
        }
        else 
        {
            std::cout << "No post installation scripts found!" << "\n";
        }
        // Storing package data
        // Adding the locations to the package files , and the packages files to DATA_DIR
        store_spm(format("%s/%s.spm",vars.SPM_DIR.c_str(),name.c_str()),dataSpmPath);

        //adding the package to the data file
        add_data();

        // Cleaning everything
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
        std::filesystem::remove_all(vars.BUILD_DIR);
        // recreating the dir 
        mkdir(vars.BUILD_DIR.c_str(), 0777);
    }
    else 
    {
        std::cout << "Package type not supported!" << std::endl;
        msg(FATAL, "Package type not supported!");
    }
    std::cout << "Your package is installed, enjoy!" << std::endl;



}


