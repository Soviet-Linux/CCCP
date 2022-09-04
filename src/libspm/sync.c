

// class stuff
#include "../../include/libspm.h"
#include "../../include/globals.h"
#include "../../include/utils.h"


void sync ()
{
    downloadRepo("all.db", ALL_DB);
    downloadRepo("VERSION",format("%s/VERSION",DATA_DIR));
}