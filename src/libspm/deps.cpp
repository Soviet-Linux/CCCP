#include <string>
#include <vector>
#include <iostream> 
#include <unistd.h>

// class stuff
#include "../../include/libspm.hpp"

// This function will check if all dependencies of a package are installed
int soviet::package::check_dependencies () 
{
    msg(DBG1,"Checking dependencies...");

    if (dependencies.size() == 0)
    {
        msg(DBG1,"No dependencies to check");
        return 1;
    }

    if (dependencies[0] == "base")
    {
        msg(INFO,"this packages requires the base system , no dependencies checking!");
        return 1;
    }
    else {
        for (auto & dep : dependencies)
        {
            // TODO: implement vesrion checking
            // is use the C function because my clangd server doesnt recognize the fucking std::filesystem thing and it bothered me 
            std::string dep_spm_path = soviet::format("%s/%s.spm",vars.SPM_DIR.c_str(),dep.c_str());
            msg(DBG2,"Checking if dependency data file : %s exists",dep_spm_path.c_str());

            if (!access((dep_spm_path).c_str(),F_OK))
            {
                msg(DBG2,"dependency %s is installed",dep.c_str());
            }
            else {
                msg(DBG2,"dependency %s is not installed",dep.c_str());
                // INstall the required dependencies
                // idk what to do 
                // can someone find an idea ? 
                // something cool
            }
        }
    }
   
    return 1;
}