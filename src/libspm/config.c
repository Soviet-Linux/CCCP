#include "stdio.h"

#include "../../include/libspm.h"
#include <stdlib.h>
#include <string.h>



/* 
This readconfig fucntion takes a config file path as agument and initianlize global variable with it.
The current implementation is stupid and bloated.
TODO: Rework it !
*/
int readConfig(char* configFilePath)
{
    
    FILE* file = fopen(configFilePath, "r"); /* should check the result */
    char line[256];
    char* key;
    char* value;
    char** kvlist;
    int count;

    while (fgets(line, sizeof(line), file)) {
        /* note that fgets don't strip the terminating \n, checking its
           presence would allow to handle lines longer that sizeof(line) */
        kvlist = split(line,'=',&count);
        if (count == 0)
        {
            msg(ERROR,"Invalid config file format");
            continue;
        }
        else {
            key = kvlist[0];
            value = kvlist[1];
            if (strcmp(key,"ROOT") == 0)
            {
                free(ROOT);
                strcpa(&ROOT,value);
            }
            else if (strcmp(key,"MAIN_DIR") == 0)
            {
                free(MAIN_DIR);
                strcpa(&MAIN_DIR,value);
            }
            else if (strcmp(key,"WORK_DIR") == 0)
            {
                free(WORK_DIR);
                strcpa(&WORK_DIR,value);
            }
            else if (strcmp(key,"INSTALLED_FILE") == 0)
            {
                free(INSTALLED_FILE);
                strcpa(&INSTALLED_FILE,value);
            }
            else if (strcmp(key,"ALL_FILE") == 0)
            {
                free(ALL_FILE);
                strcpa(&ALL_FILE,value);
            }
            else if (strcmp(key,"CONFIG_FILE") == 0)
            {
                free(CONFIG_FILE);
                strcpa(&CONFIG_FILE,value);
            }
            else if (strcmp(key,"REPOS")) 
            {
                REPOS = split(value,',',&count);
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