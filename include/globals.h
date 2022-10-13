#include "stdbool.h"

#define QUEUE_MAX 64


/*
START OF THE (sort of) CONSTANTS DECALRATIONS 
(They are not mean to be modified a lot)
*/


extern int DEBUG;
extern bool TESTING;

extern bool OVERWRITE;
extern bool QUIET;
// The filesystem root
extern char* ROOT;
// For real use it must be set to "/"

// main dir where all the files and packages will be stored and where we will work
extern char* MAIN_DIR;
// the dir where we will be building the packages and downoading the sources
//  TODO: proper dir names , but i dont have time to do it
extern char* WORK_DIR;
// the dir where the data is stored
extern char* DATA_DIR;
// where spm files of installed packages are stored
extern char* SPM_DIR;
//where we store tests and logs
extern char* LOG_DIR;
// Dir where built binaries are stored after making or after uncompressing
extern char* BUILD_DIR;
// Dir where the package sources are downloaded and built
extern char* MAKE_DIR;


//The file where a lot of data are stored
extern char* INSTALLED_DB;
extern char* ALL_DB;
// configuraton file
extern char* CONFIG_FILE;
// This is a fucking string array..., i know
extern char** REPOS;
extern int REPO_COUNT;

extern char* TEST_LOG;

// list of all formats supported by spm
extern char** FORMATS;
extern int FORMAT_COUNT;

// default format
extern char* DEFAULT_FORMAT;

/*
END OF THE CONST ZONE
*/
// In the code below we'll declare global variable that are meant to change often 
// (For inter function communication of stuff like that)

// IDK how to alloc memory efficiently to this.
// I think that allocation should be moved to the executable*
// We could also 
extern char* PACKAGE_QUEUE[];
extern int QUEUE_COUNT;

