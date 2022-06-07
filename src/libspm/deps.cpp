#include <string>
#include <vector>
#include <iostream> 
#include <unistd.h>

// class stuff
#include "../../include/libspm.hpp"

// This function will check if all dependencies of a package are installed
int soviet::package::check_dependencies () 
{
    //TODO : check if the dependencies are installed
    //
    //Print the dependencies
    if (dependencies[0] == "base")
    {
        std::cout << "this packages requires the base system , no dependencies checking!\n";
        return 1;
    }
    else {
        for (int i = 0; i < dependencies.size(); i++)
        {
            // TODO: implement vesrion checking
            // is use the C function because my clangd server doesnt recognize the fucking std::filesystem thing and it bothered me 
            if (access((DATA_DIR + dependencies[i] + ".spm").c_str(),F_OK))
            {
                std::cout << "dependency " << dependencies[i] << " is installed!\n";
            }
            else {
                std::cout << "dependency " << dependencies[i] << " is not installed!\n";
                //install automatically the needed dependency
                //To do this run install_package
                //install_package(dependencies[i]);
            }
        }
    }
   
    return 1;
}