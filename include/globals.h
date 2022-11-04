#include "stdbool.h"

#define QUEUE_MAX 64

#define MAX_REPOS 64
#define MAX_FORMATS 64


#define MAX_PATH 256

// this field may cause buffer overflow
#define MAX_CMD 64

/*
START OF THE (sort of) CONSTANTS DECALRATIONS 
(They are not mean to be modified a lot)
*/


extern int DEBUG;
extern bool TESTING;

extern bool OVERWRITE;
extern bool QUIET;
// The filesystem root
extern char ROOT[MAX_PATH];
// For real use it must be set to "/"

// main dir where all the files and packages will be stored and where we will work
extern char MAIN_DIR[MAX_PATH];
// the dir where we will be building the packages and downoading the sources
//  TODO: proper dir names , but i dont have time to do it
extern char WORK_DIR[MAX_PATH];
// the dir where the data is stored
extern char DATA_DIR[MAX_PATH];
// where spm files of installed packages are stored
extern char SPM_DIR[MAX_PATH];
//where we store test and logs
extern char LOG_DIR[MAX_PATH];
// Dir where built binaries are stored after making or after uncompressing
extern char BUILD_DIR[MAX_PATH];
// Dir where the package sources are downloaded and built
extern char MAKE_DIR[MAX_PATH];

extern char PLUGIN_DIR[MAX_PATH];

//The file where a lot of data are stored
extern char INSTALLED_DB[MAX_PATH];
extern char ALL_DB[MAX_PATH];
// configuraton file
extern char CONFIG_FILE[MAX_PATH];
// This is a fucking string array..., i know
extern char* REPOS[MAX_REPOS];
extern char REPO_ALLOC[MAX_REPOS*32];
// OKAY my guys , i just relized that well need an allocated string for all the fuckign formats
// So here we're gonne declare it.

extern int REPO_COUNT;

extern char TEST_LOG[MAX_PATH];

// list of all formats supported by spm
extern char* FORMATS[MAX_FORMATS];
extern int FORMAT_COUNT;

// default format
extern char DEFAULT_FORMAT[64];

/*
END OF THE CONST ZONE
*/
// In the code below we'll declare global variable that are meant to change often 
// (For inter function communication of stuff like that)

// IDK how to alloc memory efficiently to this.
// I think that allocation should be moved to the executable*
// We could also 
extern char* PACKAGE_QUEUE[QUEUE_MAX];
extern int QUEUE_COUNT;

