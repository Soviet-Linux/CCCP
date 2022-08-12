// This file is basically a copy of globals.h

#include "stdbool.h"

/*
START OF THE (sort of) CONSTANTS DECALRATIONS 
(They are not mean to be modified a lot)
*/

int DEBUG;
bool TESTING;

bool OVERWRITE;
bool QUIET;
// The filesystem root
char* ROOT;
// For real use it must be set to "/"

// main dir where all the files and packages will be stored and where we will work
char* MAIN_DIR;
// the dir where we will be building the packages and downoading the sources
//  TODO: proper dir names , but i dont have time to do it
char* WORK_DIR;
// the dir where the data is stored
char* DATA_DIR;
// where spm files of installed packages are stored
char* SPM_DIR;
//where we store tests and logs
char* LOG_DIR;
// Dir where built binaries are stored after making or after uncompressing
char* BUILD_DIR;
// Dir where the package sources are downloaded and built
char* MAKE_DIR;
// temp dir
char* TMP_DIR;

//The file where a lot of data are stored
char* INSTALLED_DB;
char* ALL_DB;
// configuraton file
char* CONFIG_FILE;
// This is a fucking string array..., i know
char** REPOS;
int REPO_COUNT;

char* TEST_LOG;

/*
END OF THE CONST ZONE
*/
// In the code below we'll declare global variable that are meant to change often 
// (For inter function communication of stuff like that)

// IDK how to alloc memory efficiently to this.
// I think that allocation should be moved to the executable*
// We could also 
char** PACKAGE_QUEUE;
