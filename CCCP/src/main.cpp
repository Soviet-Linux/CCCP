#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::string CURRENT_DIR = "/home/paulk/Desktop/CCCP/";

//Getting package data from file
std::vector<std::string> open_spm (std::string PName)
{
    std::streampos size;
    char * memblock;

    std::string PPath = CURRENT_DIR + "testing/" + PName + "/";
    
    std::ifstream file_spm (PPath + PName + ".spm", std::ios::in);
    std::string line;
    std::vector<std::string> pkg_info;
    if (file_spm.is_open())
    {
        while ( getline (file_spm,line) )
        {
        pkg_info.push_back(line);
        }
        file_spm.close();
        return pkg_info;
    }

    else std::cout << "Unable to open file";
    

}
int install_binary ()
{

}
//parsing data and installing package
int install_package (std::vector<std::string> PName)
{
    std::vector<std::string> pkg_info = open_spm("lolcat");
    if ( pkg_info[0] != "nothing")
    {
    //Dependencies stuff for LATER
    }
    else {
        if ( pkg_info[1] == "source") 
        {
            if (pkg_info[2] == "make")
            {
            
            }
        }
        else if ( pkg_info[1] == "binary")
        {
            install_binary();
        }
    }
}

int main () {
 
  
  return 0;
}