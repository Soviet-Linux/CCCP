#include <cstdio>
#include <ios>
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <vector>
#include <algorithm>
#include <filesystem>


#include "../include/main.h"
#include "../include/make.h"


const std::string CURRENT_DIR = "/home/paulk/Desktop/CCCP/CCCP/";
const std::string PKG_DIR = CURRENT_DIR;
const std::string DATA_DIR = CURRENT_DIR;


int main (int argc, char *argv[]) 
{
    if (argc < 2) {
        std::cout << "No arguments given. Terminating.\n";
        exit(1);
    }

    UseCase use;
    std::string PName = "";
    std::string option = argv[1];

    //parsing argument 
    // TODO : use a switch statement here
    if (option.substr(0,2) == "--") {
        if (option == "--install") {
            PName = argv[2];
            use = INSTALL;
        }
        if (option == "--create") {
            PName = argv[2];
            use = CREATE;
        }
        if (option == "--binary") {
            PName = argv[2];
            use = BINARY;
        }
        
    }
    else {
        std::cout << "No option given\n";
        std::cout << "To install a source package : \n";
        std ::cout << "Usage: cccp --install <package_name>\n\n";
        std::cout << "To create a binary package from source package package : \n";
        std ::cout << "Usage: cccp --create <package_name>\n\n";
        std::cout << "To install a binary package : \n";
        std ::cout << "Usage: cccp --binary <package_name>\n\n";
        exit(1);
    }  
    
    
    return 0;
}



//parsing data and installing package
void install_package (const std::string& PName, UseCase use)
{
    std::cout << "processing package " << PName << "\n"; 
    
    if (use == INSTALL)
    {
        //Reading package data from .spm file
        std::ifstream file_spm((PKG_DIR + PName + ".spm").c_str(), std::ios::in);
        std::stringstream buffer;
        buffer << file_spm.rdbuf();
        //parsing json data
        auto pkg_info = json::parse(buffer.str());
        std::cout  << pkg_info << "\n";

        //Checking dependencies
        if (check_dependencies(pkg_info["build_deps"], DATA_DIR))
        {
            std::cout << "dependencies are ok" << "\n";
            //making package with the download_info command from the .spm file
            make_pkg(PName, pkg_info[1], pkg_info[2], CURRENT_DIR);
            std::cout << "package built" << "\n";
        }
        else {
            std::cout << "dependencies are not ok" << "\n";
        }
        
        //Moving built binaries to their install location on the system
        for (int i = 0; i < pkg_info["install"].size(); i++)
        {
            std::string install_cmd = "mv " + CURRENT_DIR + "build/" + pkg_info["install"][i]["file"].dump() + " " + pkg_info["install"][i]["destination"].dump(); 
            std::cout << install_cmd << std::endl;
            system(install_cmd.c_str());
            system(("rm -rf " + CURRENT_DIR + "build/*").c_str());
        }   
        //Writing to pkg list
        // TODO: write pkg infos to a packages database
    }
    else if (use == CREATE) {
        
        //Reading package data from .spm file
        std::ifstream file_spm((PKG_DIR + PName + ".spm").c_str(), std::ios::in);
        std::stringstream buffer;
        buffer << file_spm.rdbuf();
        //parsing json data
        auto pkg_info = json::parse(buffer.str());

        make_pkg(PName, pkg_info[1],pkg_info[2],CURRENT_DIR);

        create_binary(PName, pkg_info[3], pkg_info[0]);
        std::cout << "binary created" << "\n";
        // if a cosmic ray flips a bit at that exact moment, dm me on discord to fix the bug
        // removed a useless jmp statement with this ;)
    }
    else if (use == BINARY){
        install_binary(PName);
    }
    else std::cout << "the use is not defined" << "\n";
}


int install_binary(std::string PName)
{
    std::string TMP_DIR = "/tmp/" + std::to_string((rand() % 100000 + 1 )) + "/";
    system(("tar -xf " + std::filesystem::current_path().string() +"/"+ PName + ".tar.gz " + TMP_DIR ).c_str());
    //Reading package data from .spm file
    std::ifstream file_spm((TMP_DIR + PName + "-bin.spm").c_str(), std::ios::in);
    std::stringstream buffer;
    buffer << file_spm.rdbuf();
    //parsing json data
    auto bin_info = json::parse(buffer.str());

    if (check_dependencies(bin_info[0],DATA_DIR)) 
    {
        std::cout << "dependencies are ok" << "\n";

    }
    else {
        std::cout << "dependencies are not ok" << "\n";
        //DO SOMETHING HERE like with dependencies
    }
    return 1;
}

void create_binary (std::string PName,std::string built_binaries,std::string dependencies)
{
    std::ofstream buildfile;
    buildfile.open((CURRENT_DIR + "build" + "/" + PName + "-bin.spm").c_str());
    buildfile << dependencies << built_binaries << "\n" ;
    buildfile.close();
    std::string cmd_archive = "(cd " + CURRENT_DIR + "build && tar -cvf " + std::filesystem::current_path().string() +"/"+ PName + "-bin.tar.gz *)" ; // TODO fix these lines
    std::cout << cmd_archive << std::endl;
    system((cmd_archive).c_str());
    system(("rm -rf " + CURRENT_DIR + "build/*").c_str());
}
int move_binaries (std::vector<std::string> install_info)
{
    for (int i = 0; i < install_info.size(); i++)
    {


    } 
    return 0;
}

