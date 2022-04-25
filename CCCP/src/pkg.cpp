#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <regex>

#include "../include/spm.h"
#include "../include/pkg.h"
#include "../include/data.h"



// this function is for uninstaling packages
void rm_pkg (const std::string& PName,const std::string& DATA_DIR,const std::string& DATA_FILE,bool DEBUG)
{
    std::string PPath = DATA_DIR + PName + ".spm";
    std::cout << "Uninstalling package" << std::endl;
    if (DEBUG) std::cout << PPath << std::endl;
    pkg_data data = open_spm(PPath);
    //remove all the files in the data["locations"]
    for (int i = 0; i < data.locations.size(); i++)
    {
        //Fuuuck it leaves the folders
        // TODO: find someone intelligent enough to fix this
        //
        try {
            if (DEBUG) std::cout << "Removing " << data.locations[i] << std::endl;
            std::filesystem::remove(data.locations[i].c_str());
        }
        catch (std::exception& e)
        {
            std::string str_e = e.what();
            if (DEBUG) std::cout << "Exception: " << str_e << '\n';
            if ( str_e.find("Directory not empty") != std::string::npos)
            {
                if(DEBUG) std::cout << "its good , no worry.\n";
            }
            else
            {
                std::cout << "This is BAD, terminating : " << str_e << '\n';
                exit(1);
            }

        }
        
    }
    //removing the entries in packages.json
    rm_pkg_data(DATA_FILE,PName);
    //remove the spm file from DATA
    std::string rm_spm_cmd = "rm -rf " + DATA_DIR + PName + ".spm";
    system((rm_spm_cmd).c_str());
}

bool check_pkg (const std::string& PPath,const std::string& DATA_DIR,bool DEBUG)
{
    
    pkg_data data = open_spm(PPath);
    //remove all the files in the data["locations"]
    for (int i = 0; i < data.locations.size(); i++)
    {
        //Fuuuck it leaves the folders
        // TODO: find someone intelligent enough to fix this
        //
         if (std::filesystem::exists(data.locations[i])) 
         {
             if (DEBUG) std::cout << data.locations[i] << " exists !" << std::endl;
         }
         else {
                std::cout << data.locations[i] << " does not exist !" << std::endl;
                return false;
         }
    }
    return true;
}
// This function is downloading and building the packages
void make_pkg (const pkg_data& pkg,const std::string& MAKE_DIR,const std::string& BUILD_DIR,const std::string& LOG_DIR,bool TESTING)
{
    // this solution with the '*' is very bad i think
    std::string package_dir = MAKE_DIR + pkg.name + "*";
    //check if  pkg.configure_info and pkg.make_info are empty
    if (pkg.configure_info.empty() && pkg.make_info.empty())
    {
        std::cout << "No make or configure info found in the package.\n";
    }
    else 
    {
        std::string make_cmd = "BUILD_ROOT="+ BUILD_DIR +"; ( cd " + package_dir + " && " + pkg.configure_info + " && " + pkg.make_info + " )";
        std::cout << make_cmd << std::endl;
        system(make_cmd.c_str());
        if (TESTING) system(("( cd "+ package_dir + " && " + pkg.test_info + " > "+ LOG_DIR + pkg.name + ".test )").c_str());
    }
    //installing the package in the build directory
    std::string install_cmd = "BUILD_ROOT="+ BUILD_DIR +"; ( cd "+ package_dir + " && " + pkg.install_info + " )";
    std::cout << install_cmd << std::endl; 
    //executing test command
    
    system(install_cmd.c_str());

    // cleaning up the build directory
    system(("rm -rf " + MAKE_DIR + pkg.name + "*").c_str());
}
