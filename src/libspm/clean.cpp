#include <string>
// class stuff
#include "../../include/libspm.hpp"

void soviet::clean()
{
    /*
        If you are  a young dev looking at this code tryng to understand it , you will see that it is not very clean
        it is in fact very bad code , so dont look at it and goo see something better.
        dont learn my bad pactices
    */

    //cleaning the build dir 
    std::string cmd_clean = format("rm -rf %s/*",vars.BUILD_DIR.c_str());
    system(cmd_clean.c_str());
    // cleaning make dir
    cmd_clean = format("rm -rf %s/*",vars.MAKE_DIR.c_str());
    system(cmd_clean.c_str());


}