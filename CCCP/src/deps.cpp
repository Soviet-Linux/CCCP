#include <string>
#include <vector>

#include "../lib/log.hpp"


// This function will check if all dependencies of a package are installed
int check_dependencies (const std::vector<std::string>& dependencies, const std::string& DATA_DIR) 
{
    //TODO : check if the dependencies are installed
    //
    //Print the dependencies
    if (dependencies[0] == "base")
    {
        std::cout << "this packages requires the base system , no dependencies checking.\n";
        return 1;
    }
    else {
        for (int i = 0; i < dependencies.size(); i++)
        {
            // TODO: implement vesrion checking
            if (std::filesystem::exists(DATA_DIR + dependencies[i] + ".spm"))
            {
                std::cout << "dependency " << dependencies[i] << " is installed.\n";
            }
            else {
                std::cout << "dependency " << dependencies[i] << " is not installed.\n";
                //install automatically the needed dependency
                //To do this run install_package
                install_package(dependencies[i]);
            }
        }
    }
   
    return 1;
}