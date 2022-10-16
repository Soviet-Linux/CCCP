#include "unistd.h"
#include "stdio.h"


// class stuff
#include "../../include/libspm.h"


//checking if package is installed and untouched
/*
Exit code signification :
0 = Good , package is installed and fine
1 = Package is not installed (Package data file is absent)
2 = Package is corrupted (package data file is here but with no location info)
3 = Package is corrupted (Some locations arent here)
*/
int check (char* dataSpmPath, char* locations[], int locationsCount)
{
    // checkinig if package data file exists
    if (access(dataSpmPath,F_OK) != 0){
        return 1;
    }
    else if (locationsCount == 0)
    {
        return 2;
    }
    else
    {
        for (int i = 0; i < locationsCount; i++)
        {
            if (access(locations[i],F_OK) != 0)
            {
                return 3;
            }
        }
        return 0;
    }
}
// simpler check that also verifies f the package is being installed 

