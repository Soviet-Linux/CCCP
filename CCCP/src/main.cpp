#include <iostream>
#include <fstream>
#include <string>
#include <vector>


std::string CURRENT_DIR = "/home/paulk/Desktop/Soviet/CCCP/";
std::string BUILD_ROOT = CURRENT_DIR + "build-root/";

//Getting package data from file
std::vector<std::string> open_spm (std::string PName)
{
    std::streampos size;
    char * memblock;

    std::string PPath = CURRENT_DIR + "testing/" + PName + "/";
    std::cout << PPath + PName + ".spm \n";
    std::ifstream file_spm (PPath + PName + ".spm", std::ios::in);
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
int install_binary ()
{

}
//parsing data and installing package
int install_package (std::string PName)
{
    std::cout << "processing package" << "\n";
    std::vector<std::string> pkg_info = open_spm("lolcat");
    std::cout << "package info parsed" << "\n";
    if ( pkg_info[0] != "nothing")
    {
    //Dependencies stuff for LATER
    }
    else {
        std::cout << "No dependencies" << "\n";
        if ( pkg_info[1] == "source") 
        {
            std::cout << "building from source" << "\n";
            if (pkg_info[2] == "make")
            {
                std::cout << "Making pkg" << "\n";
                std::string cmd_make = "( cd " + CURRENT_DIR + "testing/" + PName + "/" + PName + " && make && make DESTDIR=" + BUILD_ROOT + " install )  ";
                std::cout << cmd_make << std::endl;
                system(cmd_make.c_str());
            }
        }
        else if ( pkg_info[1] == "binary")
        {
            install_binary();
        }
    }
}

int main () {

  install_package("lolcat");

  return 0;
}