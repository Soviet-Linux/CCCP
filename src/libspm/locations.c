#include "stdio.h"
#include "string.h"
#include "stdlib.h"

// class stuff
#include "../../include/libspm.h"
#include "../../include/globals.h"
#include "../../include/utils.h"

long get_locations(char*** locations, char* loc_dir)
{
    //Get package file location
    char* files_location_cmd = format("( cd %s && find . -type f | cut -c2- ) ", BUILD_DIR);
    /*
    This way of getting locations is pretty bad , i should wite a proper way to do it
    But it works so its ok for now 
    */    
    msg(DBG2, "Getting files locations with %s ", files_location_cmd);

   
    // execute the commands
    char* loc_str = exec(files_location_cmd);
    // print the locations
    int count;
    *locations = split(loc_str,'\n',&count);

    return count;
}