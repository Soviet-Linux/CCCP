#include "stdio.h"
#include <stdlib.h>
#include <unistd.h>

// class stuff
#include "../../include/libspm.h"
#include "../../include/utils.h"

// This function will check if all dependencies of a package are installed
int check_dependencies (char ** dependencies,int dependenciesCount) 
{
    msg(DBG1,"Checking dependencies...");


    for (int i = 0; i < dependenciesCount; i++)
    {
        msg(DBG3,"Checking if %s is installed",dependencies[i]);
        if (!is_installed(dependencies[i]))
        {
            msg(DBG3, "Dependency %s is not installed",dependencies[i]);
            // TODO: we need to install the dependencie
            msg(INFO,"Installing %s",dependencies[i]);

            /*
                TODO: Find a clever way to implement automatic dependencie installing*
                In The meantime i'll implement no dep-checking.
            */
            char* tempspm = mktemp("/tmp/spm.XXXXXX");
            int t = get(dependencies[i],tempspm);
            if (t == SRC)
            {
                installSpmFile(tempspm,true);
            }
            else if (t == BIN)
            {
                installSpmFile(tempspm,true);
            }
            

            return 0;
        }
        else 
        {
            msg(DBG3, "Dependency %s is installed",dependencies[i]);
        }
    }

   
    return 0;

}