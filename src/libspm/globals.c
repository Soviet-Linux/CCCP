// This file is basically a copy of globals.h

#include "stdbool.h"
#include <stdlib.h>
#include "../../include/utils.h"
#include "../../include/globals.h"

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
char ROOT[MAX_PATH] = "/";
// For real use it must be set to "/"

// main dir where all the files and packages will be stored and where we will work
char MAIN_DIR[MAX_PATH] = "/var/cccp";
// the dir where we will be building the packages and downoading the sources
//  TODO: proper dir names , but i dont have time to do it
char WORK_DIR[MAX_PATH] = "/var/cccp/work";
// the dir where the data is stored
char DATA_DIR[MAX_PATH] = "/var/cccp/data";
// where spm files of installed packages are stored
char SPM_DIR[MAX_PATH] = "/var/cccp/spm";
//where we store tests and logs
char LOG_DIR[MAX_PATH] = "/var/cccp/log";
// Dir where built binaries are stored after making or after uncompressing
char BUILD_DIR[MAX_PATH] = "/var/cccp/work/build";
// Dir where the package sources are downloaded and built
char MAKE_DIR[MAX_PATH] = "/var/cccp/work/make";

char PLUGIN_DIR[MAX_PATH] = "/var/cccp/plugins";

//The file where a lot of data are stored
char INSTALLED_DB[MAX_PATH] = "/var/cccp/data/installed.db";
char ALL_DB[MAX_PATH] = "/var/cccp/data/all.db";
// configuraton file
char CONFIG_FILE[MAX_PATH]  = "/etc/cccp.conf";
// This is a fucking string array..., i know
char* REPOS[MAX_REPOS] = {NULL};
int REPO_COUNT = 0;

char TEST_LOG[MAX_PATH] = "/var/cccp/log/test.log";

// formats
char* FORMATS[MAX_FORMATS] = {NULL};
char REPO_ALLOC[MAX_REPOS*32];
int FORMAT_COUNT = 0;

// default format
char DEFAULT_FORMAT[64] = "ecmp";

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


