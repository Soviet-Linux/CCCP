#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "make.h"
#include "misc.h"

int make_pkg (std::string PName,std::string download_info,std::string build_info,std::string CURRENT_DIR,std::string DATA_DIR)
{
    std::string download_cmd = "( cd " + CURRENT_DIR + "sources/ && " + download_info + " )";

    std::cout << download_cmd << "\n";
    system(download_cmd.c_str());

    std::string build_cmd = "BUILD_ROOT=" + CURRENT_DIR + "build \n ( cd " + CURRENT_DIR + "sources/" + PName +" && " + build_info + " )";
    std::cout << build_cmd << std::endl;
    
    system(build_cmd.c_str());
    system(("rm -rf " + CURRENT_DIR + "sources/" + PName).c_str());
    return 0; //
}
int check_dependencies (std::string dependencies,std::string DATA_DIR) 
{
    std::vector<std::string> pkg_list = init_pkg_list(DATA_DIR);
    std::vector<std::string> pkg_deps = split(dependencies, " ");
    for (int i = 0; i < pkg_deps.size(); i++)
    {
        if (std::find(pkg_list.begin(), pkg_list.end(), pkg_deps[i]) != pkg_list.end()) std::cout << "deps all good !!!" << std::endl;
        else
        {
            std::cout << "NOT GOOD STOP RN !!!" << std::endl;
            return 1;
        }
    }
    return 0;
}
std::vector<std::string> init_pkg_list (std::string DATA_DIR)
{

    std::streampos size;
    char * memblock;

    std::vector<std::string> pkg_list;
    std::string pkg_list_file = DATA_DIR + "pkg.list";

    std::ifstream list_file((pkg_list_file).c_str(), std::ios::in);
    std::string line;
    if (list_file.is_open())
    {
        std::cout << "file opened " << "\n";
        while ( getline (list_file,line) )
        {
            pkg_list.push_back(split(line, " ")[0]);
        }
        list_file.close();
    }
    else std::cout << "the file is not opened properly " << "\n";

    for (int i = 0; i < pkg_list.size(); i++)
    {
        std::cout << pkg_list[i] << "\n";
    }
    return pkg_list;
}