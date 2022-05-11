
#include "../include/cccp.hpp"

std::vector<soviet::action> soviet::parse(int argc, char *argv[])
{
    for (int i = 1;i < argc;i++)
    {
        std::string option = argv[i];
        if (option.substr(0,1) == "-")
        {
            if (option.substr(0,2) == "--")
            {

            }
            else  
            {
                std::string subOpt = option.substr(1,option.length());
                std::cout << subOpt << std::endl;
            }
        }
        else {
            //Print the correct usage
            std::cout << "No option given\n";
            std::cout << "To install a source package : \n";
            std ::cout << "Usage: cccp --install <package_name>\n\n";
            std::cout << "To create a binary package from source package package : \n";
            std ::cout << "Usage: cccp --create <package_name>\n\n";
            std::cout << "To install a binary package : \n";
            std ::cout << "Usage: cccp --binary <package_name>\n\n";
            std::cout << "To remove a package : \n";
            std ::cout << "Usage: cccp --remove <package_name>\n\n";
            std::cout << "To check if a package is installed : \n";
            std ::cout << "Usage: cccp --check <package_name>\n\n";
            std::cout << "To initiate the data directory (Do this if you think your system is fucked because it will destroy eveything): \n";
            std ::cout << "Usage: cccp --initiate\n\n";
            exit(1);
        } 
    }
}