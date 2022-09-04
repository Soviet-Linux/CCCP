#include "../../include/utils.h"
#include <sys/stat.h>


int clean()
{
    // cleaning the work dirs
    
    return rmrf(BUILD_DIR) + rmrf(MAKE_DIR) + mkdir(BUILD_DIR,0755) + mkdir(MAKE_DIR,0755);

    


}