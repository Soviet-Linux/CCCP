#include "stdio.h"

#include "../../include/libspm.h"

#include <stdlib.h>
#include <string.h>

#include "../../include/globals.h"
#include "../../include/utils.h"


/* 
This readconfig fucntion takes a config file path as agument and initianlize global variable with it.
The current implementation is stupid and bloated.
TODO: Rework it !
*/
int readConfig(char* configFilePath)
{
    msg(DBG2,"Reading config file %s",configFilePath);
    FILE* file = fopen(configFilePath, "r"); /* should check the result */
    char line[1024];
    char* kvlist[2];
    int count;

    while (fgets(line, sizeof(line), file)) {
        /* note that fgets don't strip the terminating \n, checking its
           presence would allow to handle lines longer that sizeof(line) */
        // removing the '\n' mentioned above
        line[strlen(line)-1] = 0;
        count  = splitm(line,'=',kvlist,2);
        msg(DBG3,"Key: %s Value: %s",kvlist[0],kvlist[1]);

        if (count == 0)
        {
            msg(ERROR,"Invalid config file format");
            continue;
        }
        else {
            char* key = kvlist[0];
            char* value = kvlist[1];
            if (strcmp(key,"ROOT") == 0)
            {
                strcpy(ROOT,value);
            }
            else if (strcmp(key,"MAIN_DIR") == 0)
            {

                strcpy(MAIN_DIR,value);
            }
            else if (strcmp(key,"WORK_DIR") == 0)
            {

                strcpy(WORK_DIR,value);
            }
            else if (strcmp(key,"INSTALLED_FILE") == 0)
            {

                strcpy(INSTALLED_DB,value);
            }
            else if (strcmp(key,"ALL_FILE") == 0)
            {
                strcpy(ALL_DB,value);
            }
            else if (strcmp(key,"CONFIG_FILE") == 0)
            {

                strcpy(CONFIG_FILE,value);
            }
            else if (strcmp(key,"REPOS") == 0)
            {
                 
                REPO_COUNT = splitm(value,' ',REPOS,MAX_REPOS);
                
            }
            else if (strcmp(key,"FORMATS") == 0)
            {
                msg(DBG3,"FORMATS: %s",value);
                strcpy(REPO_ALLOC,value);
                FORMAT_COUNT = splitm(REPO_ALLOC,' ',FORMATS,MAX_FORMATS);
                for (int i = 0; i < FORMAT_COUNT; i++)
                {
                    msg(DBG3,"FORMATS: %s",FORMATS[i]);
                }
            }
            else {
                msg(ERROR,"Unknown key in config file : %s",key);
            }
            

        }

    }
    /* may check feof here to make a difference between eof and io failure -- network
       timeout for instance */

    fclose(file);

    return 0;

}
/*
smol comment to remember how the config file works :

ROOT=/
MAIN_DIR=/var/cccp
DATA_DIR=/var/cccp/data
SPM_DIR=/var/cccp/spm
...etc...

REPOS= http://localhost:8080/ 
*/