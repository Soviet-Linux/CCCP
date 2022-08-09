#include "stdio.h"


void help()
{
    //Print a cli help message for the CCCP Package manager
    printf("\n");
    printf("CCCP  - A Package Manager for Soviet Linux distros\n");
    printf("\n");
    printf("Usage: cccp [options] (parameters)\n");
    printf("\n");
    printf("Options:\n");
    printf("\n");
    printf("-h, --help\n");
    printf("\tPrint this help message\n");
    printf("\n");
    printf("-v, --version\n");
    printf("\tPrint the version of the CCCP Package Manager\n");
    printf("\n");
    printf("-l, --list\n");
    printf("\tList all available packages\n");
    printf("\n");
    printf("-i, --install\n");
    printf("\tInstall a package\n");
    printf("\n");
    printf("-u, --update\n");
    printf("\tUpdate a package\n");
    printf("\n");
    printf("-r, --remove\n");
    printf("\tRemove a package\n");
    printf("\n");
    printf("-c, --clean\n");
    printf("\tClean the package cache\n");
    printf("\n");
    printf("-s, --search\n");
    printf("\tSearch for a package\n");
    printf("\n");
    printf("-d, --download\n");
    printf("\tDownload a package\n");
    printf("\n");

}