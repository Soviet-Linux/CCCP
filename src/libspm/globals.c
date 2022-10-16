// This file is basically a copy of globals.h

#include "stdbool.h"
#include <stdlib.h>
#include "../../include/utils.h"

#define QUEUE_MAX 64

/*
START OF THE (sort of) CONSTANTS DECALRATIONS 
(They are not mean to be modified a lot)
*/

int DEBUG = 0;
bool TESTING = false;

bool OVERWRITE = false;
bool QUIET  = true;
// The filesystem root
char* ROOT = "/";
// For real use it must be set to "/"

// main dir where all the files and packages will be stored and where we will work
char* MAIN_DIR = "/var/cccp";
// the dir where we will be building the packages and downoading the sources
//  TODO: proper dir names , but i dont have time to do it
char* WORK_DIR = "/var/cccp/work";
// the dir where the data is stored
char* DATA_DIR = "/var/cccp/data";
// where spm files of installed packages are stored
char* SPM_DIR = "/var/cccp/spm";
//where we store tests and logs
char* LOG_DIR = "/var/cccp/log";
// Dir where built binaries are stored after making or after uncompressing
char* BUILD_DIR = "/var/cccp/work/build";
// Dir where the package sources are downloaded and built
char* MAKE_DIR = "/var/cccp/work/make";

char* PLUGIN_DIR = "/var/cccp/plugins";

//The file where a lot of data are stored
char* INSTALLED_DB = "/var/cccp/data/installed.db";
char* ALL_DB = "/var/cccp/data/all.db";
// configuraton file
char* CONFIG_FILE  = "/etc/cccp.conf";
// This is a fucking string array..., i know
char** REPOS = NULL;
int REPO_COUNT = 0;

char* TEST_LOG = "/var/cccp/log/test.log";

// formats
char** FORMATS;
int FORMAT_COUNT = 0;

// default format
char* DEFAULT_FORMAT = "ecmp";

/*
END OF THE CONST ZONE
*/
// In the code below we'll declare global variable that are meant to change often 
// (For inter function communication of stuff like that)

// IDK how to alloc memory efficiently to this.
// I think that allocation should be moved to the executable*
// We could also 
char* PACKAGE_QUEUE[QUEUE_MAX];
int QUEUE_COUNT;


