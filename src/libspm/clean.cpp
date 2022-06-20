#include <string>
// class stuff
#include "../../include/libspm.hpp"

void soviet::clean()
{
    /*
        If you are  young dev looking at this code tryng to understand it , you will see that it is not very clean
        it is in fact very bad code , so dont look at it and goo see something better.
        dont learn my bad pactices
    */

    //cleaning the build dir 
    std::string cmd_clean = format("rm -rf %s/*",soviet::BUILD_DIR.c_str());
    system(cmd_clean.c_str());
    // cleaning make dir
    cmd_clean = format("rm -rf %s/*",soviet::MAKE_DIR.c_str());
    system(cmd_clean.c_str());


}