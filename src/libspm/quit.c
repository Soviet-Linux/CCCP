
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../../include/libspm.h"
#include "../../include/globals.h"
#include "../../include/utils.h"


void quit(int status)
{
    if (status != 0)
    {
        msg(ERROR, "Exiting with status %d", status);
        exit(status);
    }
    return;
    
}

