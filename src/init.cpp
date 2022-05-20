#include "stdio.h"
#include "unistd.h"
#include <sys/stat.h>

#include "../include/cccp.hpp"


void soviet::init ()
{
    soviet::readConfig(CONFIG_FILE);

    // verify is all the DIR exists
    if (!access(soviet::ROOT.c_str(),F_OK)) mkdir(soviet::ROOT.c_str(),0777);
    if (!access(soviet::MAIN_DIR.c_str(),F_OK)) mkdir(soviet::MAIN_DIR.c_str(),0777);
    if (!access(soviet::DATA_DIR.c_str(),F_OK)) mkdir(soviet::DATA_DIR.c_str(),0777);
    if (!access(soviet::WORK_DIR.c_str(),F_OK)) mkdir(soviet::WORK_DIR.c_str(),0777);
    if (!access(soviet::SPM_DIR.c_str(),F_OK)) mkdir(soviet::SPM_DIR.c_str(),0777);
    if (!access(soviet::LOG_DIR.c_str(),F_OK)) mkdir(soviet::LOG_DIR.c_str(),0777);
    if (!access(soviet::BUILD_DIR.c_str(),F_OK)) mkdir(soviet::BUILD_DIR.c_str(),0777);
    if (!access(soviet::MAKE_DIR.c_str(),F_OK)) mkdir(soviet::MAKE_DIR.c_str(),0777);

    // init data
    if (access(DATA_FILE.c_str(),F_OK)) soviet::init_data();

    // Do other stuff if you want


    
}