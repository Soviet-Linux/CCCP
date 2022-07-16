#include <string>
#include <vector>
#include <iostream> 
#include <unistd.h>

// class stuff
#include "../../include/libspm.hpp"

// This function will check if all dependencies of a package are installed
int soviet::package::check_dependencies () 
{
    if(DEBUG) std::cout << "Checking dependencies..." << std::endl;

    if (dependencies.size() == 0)
    {
        if(DEBUG) std::cout << "No dependencies to check" << std::endl;
        return 1;
    }

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
            std::string dep_spm_path = soviet::format("%s%s.spm",SPM_DIR.c_str(),dependencies[i].c_str());
            if (DEBUG) std::cout << "Checking if dependency data file : " << dep_spm_path << " exists" << std::endl;

            if (!access((dep_spm_path).c_str(),F_OK))
            {
                std::cout << "dependency " << dependencies[i] << " is installed!\n";
            }
            else {
                std::cout << "dependency " << dependencies[i] << " is not installed!\n";
                // INstall the required dependencies
                // idk what to do 
                // can someone find an idea ? 
                // something cool
            }
        }
    }
   
    return 1;
}