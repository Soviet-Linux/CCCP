#include "stdio.h"
#include "unistd.h"
#include <sys/stat.h>

#include "../../include/libspm.h"
#include "../../include/utils.h"
#include "../../include/globals.h"
#include "../../include/data.h"


void init()
{



    clean();
    readConfig(CONFIG_FILE);
    // verify is all the DIR exists
    if (access(ROOT,F_OK)) mkdir(ROOT,0777);
    if (access(MAIN_DIR,F_OK)) mkdir(MAIN_DIR,0777);
    if (access(DATA_DIR,F_OK)) mkdir(DATA_DIR,0777);
    if (access(WORK_DIR,F_OK)) mkdir(WORK_DIR,0777);
    if (access(SPM_DIR,F_OK)) mkdir(SPM_DIR,0777);
    if (access(LOG_DIR,F_OK)) mkdir(LOG_DIR,0777);
    if (access(BUILD_DIR,F_OK)) mkdir(BUILD_DIR,0777);
    if (access(MAKE_DIR,F_OK)) mkdir(MAKE_DIR,0777);

    // init data
    // TODO: do some stuff for the data
    if (access(INSTALLED_DB,F_OK) != 0) init_data(INSTALLED_DB);


    if (access(ALL_DB, F_OK))
    {
        msg(ERROR, "Global package data file not found, to download it use -s option!");
    }
    
    // Do other stuff if you want


    
}