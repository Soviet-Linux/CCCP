#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>

std::string CURRENT_DIR = "/home/paulk/Desktop/Soviet/CCCP/";
std::string DATA_DIR = CURRENT_DIR;

int install_package (std::string PName);
int install_binary ();
std::vector<std::string> open_spm (std::string PName);
std::vector<std::string> init_pkg_list ();
std::vector<std::string> split(std::string str, std::string token);

std::vector<std::string> installed_packages = init_pkg_list ();

int main () {

  install_package("lolcat");

  return 0;
}

//Getting package data from file
std::vector<std::string> open_spm (std::string PPath)
{
    std::streampos size;
    char * memblock;

    std::ifstream file_spm (PPath, std::ios::in);
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

std::vector<std::string> init_pkg_list ()
{

    std::streampos size;
    char * memblock;

    std::vector<std::string> pkg_list;

    std::ifstream list_file ( DATA_DIR + "pkg.list", std::ios::in);
    std::string line;
    if (list_file.is_open())
    {
        std::cout << "file opened " << "\n";
        while ( getline (list_file,line) )
        {
            pkg_list.push_back(line);
        }
        list_file.close();
    }
    else std::cout << "ITS BAD"<< "\n";

    return pkg_list;
}

//parsing data and installing package
int install_package (std::string PName)
{
    std::cout << "processing package" << "\n";
    std::vector<std::string> pkg_info = open_spm(PName + ".spm");
    std::vector<std::string> pkg_deps = split(pkg_info[0], " ");
    std::cout << "package info parsed" << "\n";
    if ( pkg_info[0] != "")
    {
        for (int i = 1; i < pkg_deps.size(); i++)
        {
            if (std::find(installed_packages.begin(), installed_packages.end(), pkg_deps[i]) != installed_packages.end()) std::cout << "deps all good !!!" << std::endl;
            else
            {
                std::cout << "NOT GOOD STOP RN !!!" << std::endl;
                
                // Dependencies stuff to complicated  
            }
        }
        
    }
    else std::cout << "No dependencies" << "\n";      

    //END OF DEPENDENCIES CHECK
    
    std::string download_pkg = "( cd " + CURRENT_DIR + "sources/ && " + pkg_info[1] + " )";

    system(download_pkg.c_str());

    std::string build_cmd = "BUILD_ROOT=" + CURRENT_DIR + "build \n ( cd " + CURRENT_DIR + "sources/" + PName +" && " + pkg_info[2] + " )";
    std::cout << build_cmd << std::endl;
    
    system(build_cmd.c_str());
    system(("rm -rf " + CURRENT_DIR + "sources/" + PName).c_str());

    std::vector<std::string> install_info = split(pkg_info[3],"|");
    for (int i = 0; i < install_info.size(); i++)
    {
        std::string install_cmd = "mv " + CURRENT_DIR + "build/" + split(install_info[i], " ")[0] + " " + split(install_info[i], " ")[1]; 
        std::cout << install_cmd << std::endl;
        system(install_cmd.c_str());
        system(("rm -rf " + CURRENT_DIR + "build/*").c_str());
    }


    
}

std::vector<std::string> split(std::string str, std::string token){
    std::vector<std::string>result;
    while(str.size()){
        int index = str.find(token);
        if(index!=std::string::npos){
            result.push_back(str.substr(0,index));
            str = str.substr(index+token.size());
            if(str.size()==0)result.push_back(str);
        }else{
            result.push_back(str);
            str = "";
        }
    }
    return result;
}