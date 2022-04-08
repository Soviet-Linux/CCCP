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
#include "../include/misc.h"
#include "../include/make.h"

const std::string CURRENT_DIR = "/home/paulk/Desktop/CCCP/CCCP/";
const std::string PKG_DIR = CURRENT_DIR;
const std::string DATA_DIR = CURRENT_DIR;


int main (int argc, char *argv[]) 
{
    if (argc < 2) {
        std::cout << "No arguments given.It will run buts its not ok !\n";
    }
    else { 
        std::cout << "Arguments given " << argc << "\n"; 
    }

    UseCase use;
    std::string PName = "";
    std::string option = argv[1];

    std::cout << "Option is " << option << "\n";

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
        if (option == "--debug") {
            std::cout << "Debug mode\n";
        }
        
    }

    
    install_package(PName,use);
    
    return 0;
}



//parsing data and installing package
void install_package (const std::string& PName, UseCase use)
{
    std::cout << "processing package " << PName << "\n"; 
    
    if (use == INSTALL)
    {
        auto pkg_info = open_spm(PKG_DIR + PName + ".spm");
        std::cout << "Package info is " << pkg_info << "\n";
        if (check_dependencies(pkg_info["build_deps"], DATA_DIR))
        {
            std::cout << "dependencies are ok" << "\n";
            make_pkg(PName, pkg_info[1], pkg_info[2], CURRENT_DIR);
            std::cout << "package built" << "\n";
        }
        else {
            std::cout << "dependencies are not ok" << "\n";
        }

        std::vector<std::string> install_info = split(pkg_info[3], "|"); // TODO: use yaml/json for pkg formats instead of a custom one
        for (int i = 0; i < install_info.size(); i++)
        {
            std::string install_cmd = "mv " + CURRENT_DIR + "build/" + split(install_info[i], " ")[0] + " " + split(install_info[i], " ")[1]; 
            std::cout << install_cmd << std::endl;
            system(install_cmd.c_str());
            system(("rm -rf " + CURRENT_DIR + "build/*").c_str());
        }   
        //Writing to pkg list
        std::ofstream pkg_list;
        pkg_list.open(DATA_DIR + "pkg.list");
        pkg_list << pkg_info[3] <<  "\n";  
    }
    else if (use == CREATE) {
        
        std::vector<std::string> pkg_info = open_spm(PKG_DIR + PName + ".spm");
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
    std::vector<std::string> bin_info = open_spm(TMP_DIR + PName + "-bin.spm");
    if (check_dependencies(bin_info[0],DATA_DIR)) 
    {
        std::cout << "dependencies are ok" << "\n";
        move_binaries(split(bin_info[1],"|"));
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
        std::string install_cmd = "mv " + CURRENT_DIR + "build/" + split(install_info[i], " ")[0] + " " + split(install_info[i], " ")[1]; 
        std::cout << install_cmd << std::endl;
        system(install_cmd.c_str());
    } 
    return 0;
}

