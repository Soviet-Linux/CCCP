#include "stdio.h"
#include "unistd.h"
#include <sys/stat.h>

#include "../../include/libspm.hpp"


void soviet::init ()
{
    clean();
    readConfig(vars.CONFIG_FILE);
    // verify is all the DIR exists
    if (access(vars.ROOT.c_str(),F_OK)) mkdir(vars.ROOT.c_str(),0777);
    if (access(vars.MAIN_DIR.c_str(),F_OK)) mkdir(vars.MAIN_DIR.c_str(),0777);
    if (access(vars.DATA_DIR.c_str(),F_OK)) mkdir(vars.DATA_DIR.c_str(),0777);
    if (access(vars.WORK_DIR.c_str(),F_OK)) mkdir(vars.WORK_DIR.c_str(),0777);
    if (access(vars.SPM_DIR.c_str(),F_OK)) mkdir(vars.SPM_DIR.c_str(),0777);
    if (access(vars.LOG_DIR.c_str(),F_OK)) mkdir(vars.LOG_DIR.c_str(),0777);
    if (access(vars.BUILD_DIR.c_str(),F_OK)) mkdir(vars.BUILD_DIR.c_str(),0777);
    if (access(vars.MAKE_DIR.c_str(),F_OK)) mkdir(vars.MAKE_DIR.c_str(),0777);

    // init data
    if (access(vars.INSTALLED_FILE.c_str(),F_OK)) init_data();
    if (access(vars.ALL_FILE.c_str(), F_OK))
    {
        msg(level::ERROR, "Global package data file not found, to download it use -s option!");
    }
    mkdir(vars.TMP_DIR.c_str(),0777);
    // Do other stuff if you want


    
}