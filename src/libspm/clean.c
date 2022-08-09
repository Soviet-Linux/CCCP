#include "../../include/utils.h"


int clean()
{
    // cleaning the work dirs
    
    return rmrf(BUILD_DIR) + rmrf(MAKE_DIR);


}