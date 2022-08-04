#include "../../include/libspm.h"

int clean()
{
    // cleaning the work dirs
    
    return rmrf(BUILD_DIR) + rmrf(MAKE_DIR);


}