#include "stdio.h"
#include "unistd.h"

#include "../../include/libspm.h"
#include "../../include/shared.h"
#include "../../include/spm.h"

int test_spm(char* TEST_SPM);

int main(int argc,char** argv)
{
    if (argc < 2)
    {
        printf("Usage: spm-tester <spm-file>\n");
        return 1;
    }
    char* path = argv[1];
    // check if file exists
    if (access(path,F_OK) != 0)
    {
        printf("File %s does not exist\n",path);
        return 1;
    }

    test_spm(path);

    printf("Launching spm-tester...\n");
    printf("Installing %s\n",path);
    installSpmFile(path,0);
    return 0;
}

int test_spm(char* TEST_SPM)
{
    int EXIT = 0;

    printf("Testing spm functions\n");



    struct package t_pkg;

    printf("Parsing package file \n");

    open_spm(TEST_SPM,&t_pkg);

    printf("  %s => %s %s\n",t_pkg.name,t_pkg.version,t_pkg.type);
    // print info


    return EXIT;
}