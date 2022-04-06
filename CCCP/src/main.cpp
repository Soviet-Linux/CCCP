#include <ios>
#include <iostream>
#include <fstream>
#include <string>
#include <type_traits>
#include <vector>
#include <algorithm>
#include <filesystem>

#include "../include/main.h"
#include "../include/misc.h"
#include "../include/make.h"

const std::string USER = "paulk"; // TODO: change this
const std::string CURRENT_DIR = string_format("/home/%s/Desktop/CCCP/CCCP/", USER);
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
        
    }

    std::cout << "PName is " << PName << "\n";
    install_package(PName,use);
    
    return 0;
}



//parsing data and installing package
void install_package (const std::string& PName, UseCase use)
{
    std::cout << "processing package " << PName << "\n";
    std::cout << "package info parsed" << "\n";
    
    if (use == INSTALL)
    {
        const std::vector<std::string>& pkg_info = open_spm(PName + ".spm", PKG_DIR);
        if (check_dependencies(const_cast<std::string&>(pkg_info[0]), DATA_DIR))
        {
            std::cout << "dependencies are ok" << "\n";
            make_pkg(PName, pkg_info[1], pkg_info[2], CURRENT_DIR);
            std::cout << "package built" << "\n";
        }
        else std::cout << "dependencies are not ok" << "\n";

        std::vector<std::string> install_info = split(pkg_info[3], "|"); // TODO: use yaml/json for pkg formats instead of a custom one
        for (int i = 0; i < install_info.size(); i++)
        {
            std::string install_cmd = "mv " + CURRENT_DIR + "build/" + split(install_info[i], " ")[0] + " " + split(install_info[i], " ")[1]; 
            std::cout << install_cmd << std::endl;
            system(install_cmd.c_str());
            system(("rm -rf " + CURRENT_DIR + "build/*").c_str());
        }   
    }
    else if (use == CREATE) {
        
        std::vector<std::string> pkg_info = open_spm(PName + ".spm",PKG_DIR);
        make_pkg(PName, pkg_info[1],pkg_info[2],CURRENT_DIR);

        create_binary(PName, pkg_info[3], pkg_info[0]);
        std::cout << "binary created" << "\n";
        // if a cosmic ray flips a bit at that exact moment, dm me on discord to fix the bug
        // removed a useless jmp statement with this ;)
    }
    else if (use == BINARY){
        system(("tar -xf " + PName + ".tar.gz").c_str());
        // NOT FINISHED AT ALL DONT CHANGE THINGS HERE
    }
    else std::cout << "ERROR" << "\n";
}

void install_binary()
{
    std::system(string_format("tar -xf %s/", std::filesystem::current_path().string().c_str()).c_str());
}

void create_binary (std::string PName,std::string built_binaries,std::string dependencies)
{
    std::ofstream buildfile;
    buildfile.open((CURRENT_DIR + "build" + "/" + PName + "-bin.spm").c_str());
    buildfile << built_binaries << "\n" << dependencies;
    buildfile.close();
    std::string cmd_archive = "(cd " + CURRENT_DIR + "build && tar -cvf " + std::filesystem::current_path().string() +"/"+ PName + "-bin.tar.gz *)" ; // TODO fix these lines
    std::cout << cmd_archive << std::endl;
    system((cmd_archive).c_str());
    system(("rm -rf " + CURRENT_DIR + "build/*").c_str());
}

