#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// class stuff

#include "../../include/globals.h"
#include "../../include/utils.h"

// This function is moving the binaries to the correct locations
void move_binaries(char** locations,long loc_size)
{
    /*

    // check if all locations are empty
    msg(DBG1,"Checking if all locations are empty");Z
     For now , this checking part is completely useless 
     i plan to use it to do some checks on the system.
     I will leave it like that for now 
    */
    for (int i = 0; i < loc_size; i++)
    {
        char* dest_loc = calloc(strlen(locations[i]+strlen(ROOT))+1,sizeof(char));
        sprintf(dest_loc,"%s/%s",ROOT,locations[i]);
        char* build_loc = calloc(strlen(BUILD_DIR)+strlen(locations[i])+1,sizeof(char));
        sprintf(build_loc,"%s/%s",BUILD_DIR,locations[i]);

        if (!(access(dest_loc,F_OK) == 0))
        {   if (locations[i] == NULL)
            {
                msg(ERROR,"Location is NULL");
                exit(1);
            }
            //printf("[%d/%ld] - %s/%s to => %s\n",i,loc_size, BUILD_DIR,locations[i],dest_loc);
            //fflush(stdout);
            // now that we know it is empty , mov the stuff
            //msg(DBG1,"Moving %s/%s to %s",BUILD_DIR,locations[i],dest_loc);
            
            mvsp(build_loc,dest_loc);
            free(build_loc);
            
            //smsg(DBG1,"Moved %s/%s to %s",BUILD_DIR,locations[i],dest_loc);
        }
        else 
        {

            msg(WARNING,"%s is already here",dest_loc);

            if (OVERWRITE) 
            {
                rename(build_loc,dest_loc);
            }
            else {
                msg(FATAL, "Terminating the program");
            }
        }
        free(dest_loc);
    }
    return;


}