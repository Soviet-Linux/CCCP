#include "stdio.h"
#include "unistd.h"
#include <sys/stat.h>

#include "../../include/libspm.hpp"


void soviet::init ()
{
    readConfig(CONFIG_FILE);
    // verify is all the DIR exists
    if (access(ROOT.c_str(),F_OK)) mkdir(ROOT.c_str(),0777);
    if (access(MAIN_DIR.c_str(),F_OK)) mkdir(MAIN_DIR.c_str(),0777);
    if (access(DATA_DIR.c_str(),F_OK)) mkdir(DATA_DIR.c_str(),0777);
    if (access(WORK_DIR.c_str(),F_OK)) mkdir(WORK_DIR.c_str(),0777);
    if (access(SPM_DIR.c_str(),F_OK)) mkdir(SPM_DIR.c_str(),0777);
    if (access(LOG_DIR.c_str(),F_OK)) mkdir(LOG_DIR.c_str(),0777);
    if (access(BUILD_DIR.c_str(),F_OK)) mkdir(BUILD_DIR.c_str(),0777);
    if (access(MAKE_DIR.c_str(),F_OK)) mkdir(MAKE_DIR.c_str(),0777);

    // init data
    if (access(INSTALLED_FILE.c_str(),F_OK)) init_data();
    if (access(ALL_FILE.c_str(), F_OK))
    {
        msg(level::ERROR, "Global package data file not found, to download it use -s option!");
    }

    // Do other stuff if you want


    
}