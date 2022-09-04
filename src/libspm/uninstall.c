#include "stdio.h"
#include "unistd.h"

//the class file 
#include  "../../include/libspm.h"
#include "../../include/utils.h"
#include "../../include/spm.h"
#include "../../include/data.h"

/*
To uninstall packages , we'll use the location data ( all the files that were installed by the program )
If you want more detail go to store_spm() in spm.cpp
So we have all list of all thoses file in a json array inside the package's spm file in DATA_DIR
(each package have a spm file in the DATA_DIR , and each spm file contains a json array of all the files that were installed by the program)
And we will cycle through all the files in the json array and remove them from the system.

This is simple . It could be simpler but really i dont know how .
So dont touch this , except if there are a critical bug or an important missing feature.
*/
// this function is for uninstaling packages
int uninstall(char* name)
{       
    char* dataSpmPath = format("%s/%s.spm",DATA_DIR,name);
    // verify if the package is installed
    msg(DBG3, "Verifying if the package is installed with %s" , dataSpmPath);
    // check if SPM_FILE exists
    if (!(access(dataSpmPath,F_OK) == 0)) {
        return -1;
    }

    struct package r_pkg;
    open_spm(dataSpmPath,&r_pkg);



    msg(DBG3,"Found %d locations",r_pkg.locationsCount);
    //remove all the files in the data["locations"]
    for (int i = 0; i < r_pkg.locationsCount; i++)
    {
        // debug
        msg(DBG3,"Removing %s",r_pkg.locations[i]);
        msg(DBG3,"Remove exited with code %d",remove(r_pkg.locations[i]));   
    }
    //removing the entries in packages.json
    remove_data(INSTALLED_DB,r_pkg.name);

    //remove the spm file from DATA_DIR
    remove(dataSpmPath);
    return 0;
}
