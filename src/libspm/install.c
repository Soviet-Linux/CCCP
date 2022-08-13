


// class stuff
#include "../../include/libspm.h"
#include "../../include/utils.h"
#include "../../include/spm.h"
#include <stdlib.h>


/*
In this function we are installing source or binary packages.
Source packages are archive files containing the source code of the package ,the post install script and an .spm file for the install commands.
Binary packages are archive files containing the compiled binary files of the package , the post install script and an .spm file for the install commands.
*/


// parsing data and installing package archive (with sources)

int installSpmFile(char* spm_path)
{
    struct package pkg;

    open_spm(spm_path, &pkg);

    pkg.name; // add this shit to the PKG_QUEUE ARRAY

    

    // check dependencies
    //check_dependencies();


    return 0;
}

int installSpmBinary()
{
    return 0;
}
















