
// class stuff
#include "../include/cccp.hpp"

void soviet::help()
{
    //Print a cli help message
    std::cout << "cccp - A simple package manager for linux" << std::endl;
    std::cout << "Usage: cccp [options] [arguments]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "-i, --install\t\tInstall a package" << std::endl;
    std::cout << "-r, --remove\t\tUninstall a package" << std::endl;
    std::cout << "-l, --list\t\tList all packages" << std::endl;
    std::cout << "-c, --check\t\tCheck if a package is installed" << std::endl;
    std::cout << "-h, --help\t\tPrint this help message" << std::endl;
    std::cout << "-s, --sync\t\tSynchronize package file" << std::endl;

}