
// class stuff
#include "../../include/libspm.hpp"

void soviet::help()
{
    //Print a cli help message
    std::cout << "CCCP - A package manager for Soviet linux distro" << std::endl;
    std::cout << "Usage: cccp [options] [arguments]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "-i, --install\t\tInstall a package from the repos!" << std::endl;
    std::cout << "-p, --package\t\tInstall a package file!" << std::endl;
    std::cout << "-r, --remove\t\tUninstall a package!" << std::endl;
    std::cout << "-l, --list\t\tList all packages!" << std::endl;
    std::cout << "-c, --check\t\tCheck if a package is installed!" << std::endl;
    std::cout << "-h, --help\t\tPrint this help message!" << std::endl;
    std::cout << "-s, --sync\t\tSynchronize package file!" << std::endl;
    std::cout << "-v, --version\t\tPrint the version of the package manager!" << std::endl;
    std::cout << "-d, --debug\t\tPrint debug messages!" << std::endl;
    std::cout << "-u, --update\t\tUpdate the package manager!" << std::endl;
    std::cout << "-t, --testing\t\tTesting mode!" << std::endl;
    


}