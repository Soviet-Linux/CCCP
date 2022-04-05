#include <ios>
#include <iostream>
#include <fstream>
#include <string>
#include <type_traits>
#include <vector>
#include <algorithm>
#include <filesystem>

#include "main.h"


std::string CURRENT_DIR = "/home/paulk/Desktop/SovietRepo/Soviet/CCCP/";
std::string PKG_DIR = CURRENT_DIR;
std::string DATA_DIR = CURRENT_DIR;


int main (int argc, char *argv[]) 
{
  if (argc < 2) {

        std::cout << "No arguments given.It will run buts its not ok !\n";

      }
  else { std::cout << "Arguments given " << argc << "\n"; }

  int use = 0;
  std::string PName = "";
  

  std::string option = argv[1];

  std::cout << "Option is " << option << "\n";

  if (option.substr(0,2) == "--") {
        if (option == "--install") {
            PName = argv[2];
            use = 1;
        }
        if (option == "--create") {
            PName = argv[2];
            use = 2;
        }
        
  }
  std::cout << "PName is " << PName << "\n";
  install_package(PName,use);
  return 0;
}

//Getting package data from file
std::vector<std::string> open_spm (std::string PPath)
{
    std::streampos size;
    char * memblock;
    PPath = PKG_DIR + PPath;
    std::cout << PPath << "\n";
    std::ifstream file_spm(PPath.c_str(), std::ios::in);
    std::string line;
    std::vector<std::string> pkg_info;
    if (file_spm.is_open())
    {
        std::cout << "file opened " << "\n";
        while ( getline (file_spm,line) )
        {
        pkg_info.push_back(line);
        }
        file_spm.close();
        return pkg_info;
    }

    else {
        std::cout << "Unable to open file \n";
        return std::vector<std::string> () ;
    }

    

}

//parsing data and installing package
int install_package (std::string PName,int use)
{

    std::cout << "processing package " << PName << "\n";
    std::vector<std::string> pkg_info = open_spm(PName + ".spm");
    std::vector<std::string> pkg_deps = split(pkg_info[0], " ");
    std::cout << "package info parsed" << "\n";
    if ( pkg_info[0] != "") check_dependencies(pkg_info[0]);

    else std::cout << "No dependencies" << "\n";      

    //END OF DEPENDENCIES CHECK
    

    if (use == 1)
    {
        make_pkg(PName, pkg_info[1],pkg_info[2],CURRENT_DIR,DATA_DIR);
        std::vector<std::string> install_info = split(pkg_info[3],"|");
        for (int i = 0; i < install_info.size(); i++)
        {
            std::string install_cmd = "mv " + CURRENT_DIR + "build/" + split(install_info[i], " ")[0] + " " + split(install_info[i], " ")[1]; 
            std::cout << install_cmd << std::endl;
            system(install_cmd.c_str());
            system(("rm -rf " + CURRENT_DIR + "build/*").c_str());
        }   
    }
    else if (use == 2) {
        create_binary(PName,pkg_info[3]);
    }
    else std::cout << "ERROR" << std::endl;

    return 0;

    


    
}

std::string create_binary (std::string PName,std::string built_binaries)
{
    std::ofstream buildfile;
    buildfile.open((CURRENT_DIR + "build" + "/" + PName + "-bin.spm").c_str());
    buildfile << built_binaries;
    buildfile.close();
    std::string cmd_archive = "tar -cvf " + CURRENT_DIR + "build/" + PName + "-bin.tar " + CURRENT_DIR + "build/" + PName + "-bin.spm";
    system(("tar -cvf " + PName + ".tar.gz " + CURRENT_DIR + "build/*").c_str());
    return 0;
}

