#include <stdio.h>
#include <vector>
#include <string>

// class stuff
#include "../../include/libspm.hpp"

// This function is moving the binaries to the correct locations
void soviet::package::move_binaries()
{

    std::string move_cmd = format("cp -rfl %s/* %s",BUILD_DIR.c_str(),ROOT.c_str());
    if (DEBUG) std::cout << "Executing move cmd :" << move_cmd << std::endl;
    if (system(move_cmd.c_str()) != 0)
    {
        std::cout << "Error moving binaries, its is bad !" << std::endl;
        exit(1);
    }

}