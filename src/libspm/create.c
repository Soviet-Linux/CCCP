
#include "../../include/libspm.h"
#include "../../include/utils.h"
#include <stdlib.h>

int createBinary(char* spm_path,char* bin_path)
{
    struct package pkg;

    open_pkg(spm_path, &pkg);

    PACKAGE_QUEUE[QUEUE_COUNT] = pkg.name; // add this shit to the PKG_QUEUE ARRAY
    QUEUE_COUNT++;
    msg(DBG1,"Added %s to the queue",pkg.name);



    /* 
        here we have some problems:
            The legacy package dir was in MAKE_DIR/$NAME-$VERSION
        Should we keep it or not ? 
        I choose for compatibility reasons to keep it.
        If soeone wants to chnage this you can vote here :
         - keep it : 1
         - chnage it  : 0
    */
    char* package_dir = format("%s/%s-%s",MAKE_DIR,pkg.name,pkg.version);

    // making the package
    msg(DBG1,"Making %s",pkg.name);
    make(package_dir,&pkg);
    msg(DBG1,"Making %s done",pkg.name);


    // getting locations
    msg(DBG1,"Getting locations for %s",pkg.name);
    pkg.locationsCount = get_locations(&pkg.locations,BUILD_DIR);

    // creating spm file in BUILD_DIR
    msg(DBG1,"Creating spm file for %s",pkg.name);
    create_pkg(format("%s/%s.spm",BUILD_DIR,pkg.name),&pkg,0);

    // compressing stuff to package archive
    msg(DBG1,"Compressing binaries for %s",pkg.name);
    create_archive(BUILD_DIR,bin_path);

    clean();

    return 0;
}

int create_archive(char* DIR,char* out_path)
{
    char* archive_cmd = format("( cd %s && tar -czf %s . )",DIR,out_path);
    msg(DBG2,"Creating archive with %s",archive_cmd);

    return system(archive_cmd);
}