#include "stdio.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dlfcn.h>

#include "../include/globals.h"
#include "../include/libspm.h"
#include "../include/data.h"
#include "../include/utils.h"
#include "../include/libspm.h"


#define STATIC

extern int open_spm(char* path,struct package* pkg);
extern int open_ecmp(char* path,struct package* pkg);

extern int create_spm(char* path,struct package* pkg);
extern int create_ecmp(char* path,struct package* pkg);

char* names[5] = {"pkg1","pkg2","pkg3","pkg4","pkg5"};
char* versions[5] = {"1.1.0","2.0.8","6.8.7","7.0","5.20"};
char* types[5] = {"bin","src","src","bin","src"};

#define l_d_count 5
#define l_f_count 12
char* l_dirs[l_d_count] = {"b","b/d","s","s/j","s/j/k"};
char* l_files[l_f_count] = {"w","b/d/e","a","d","b/y","b/c","b/f","s/j/k/z","s/j/k/x","s/j/k/c","s/j/k/v","s/j/k/b"};

char** list_of_stuff  = NULL;
int list_of_stuff_count = 0;

int test_spm();
int test_data ();
int test_ecmp();
int test_move();
int test_get();
int test_split();
int test_config();

char* assemble(char** list,int count);

int main(int argc, char const *argv[])
{

    if (argc  < 2)
    {
        printf("No arguments provided\n");
        return 1;
    }

    DEBUG = 3;
    QUIET = false;
    OVERWRITE = true;

    

    if (strcmp(argv[1],"spm") == 0)
    {
        return test_spm();
    }
    else if (strcmp(argv[1],"data") == 0)
    {
        return test_data();
    }
    else if (strcmp(argv[1],"ecmp") == 0)
    {
        return test_ecmp();
    }
    else if (strcmp(argv[1],"all") == 0)
    {
        int ret = 0;
        ret += test_spm();
        ret += test_data();
        ret += test_ecmp();
        return ret;
    }
    else if (strcmp(argv[1],"install") == 0)
    {
        init();
        installSpmFile(argv[2],0);
        return 0;
    }
    else if (strcmp(argv[1],"move") == 0)
    {
        return test_move();
    }
    else if (strcmp(argv[1],"help") == 0)
    {
        printf("Usage: test [spm|data|ecmp|all|help|install]\n");
        return 0;
    }
    else if (strcmp(argv[1],"split") == 0)
    {
        return test_split();
    }

    else if (strcmp(argv[1],"config") == 0)
    {
        return test_config();
    }
    else if (strcmp(argv[1],"get") == 0)
    {
        return test_get();
    }
    else
    {
        printf("Invalid argument\n");
        return 1;
    }


}

int test_move()
{
    
    init();
    printf("Testing move\n");
    strcpy(ROOT,"/tmp/spm-testing");
    strcpy(BUILD_DIR,"/tmp/spm-testing/old");
    printf("Creating directories\n");
    rmrf(ROOT);
    printf("Creating directories\n");
    mkdir(ROOT,0777);
    printf("Creating directories\n");
    mkdir(BUILD_DIR,0777);
    printf("Creating directories\n");

    printf("Creating test dirs\n");
    //make all dirs
    for (int i = 0; i < l_d_count; i++)
    {
        printf("Creating %s\n",l_dirs[i]);
        char* dir = malloc(256);
        sprintf(dir,"%s/%s",BUILD_DIR,l_dirs[i]);
        mkdir(dir,0777);
        free(dir);
    }
    printf("creating test files\n");
    // make all files
    for (int i = 0; i < l_f_count; i++)
    {
        printf("Creating %s\n",l_files[i]);
        char* path = malloc(256);
        sprintf(path,"%s/%s",BUILD_DIR,l_files[i]);
        printf("Path: %s\n",path);
        FILE* f = fopen(path,"w");
        fclose(f);
        free(path);
    }

    // get all the files with get_locatins
    char** end_locations;
    int end_count = get_locations(&end_locations,BUILD_DIR);

    // print end locations
    printf("End locations:\n");
    for (int i = 0; i < end_count; i++)
    {
        printf("%s\n",end_locations[i]);
    }


    move_binaries(end_locations,8);
    free(*end_locations);
    free(end_locations);
    //freearr((void***)&end_locations,end_count+1);

    quit(0);
    return 0;
}


int test_split()
{
    char split_str[] = "Hello,World,This,is,a,test";

    printf("cutils test\n");

    printf("Testing split\n");
    char** split_list =calloc(16, sizeof(char*));
    int count = split(split_str,',',split_list);

        // print list
    printf("split : printing list\n");
    for (int i = 0; i < count; i++)
    {
        printf("  %s\n",split_list[i]);
    }
    free(split_list);
    return 0;
}

int test_config()
{
    printf("cutils test\n");

    printf("Testing config\n");
    return readConfig(CONFIG_FILE);

}


int test_get()
{
    strcpy(ALL_DB,"tests/all.db");

    init();
    int EXIT = 0;

    struct package t_pkg;
    t_pkg.name = "test";
    t_pkg.type = "src";

    char* fmt = get(&t_pkg,"test");
    
    // print fmt and all package info
    printf("fmt: %s\n",fmt);
    printf("name: %s\n",t_pkg.name);
    printf("version: %s\n",t_pkg.version);
    printf("type: %s\n",t_pkg.type);


    return 0;

}
int test_data ()
{
    init();
    int EXIT = EXIT_SUCCESS;


    printf("Removing test.db\n");
    remove(ALL_DB);

    printf("Creating test database...\n");

    init_data(ALL_DB);
    
    printf("Adding data to test database...\n");
    for (int i = 0; i < 5; i++)
    {
        struct package a_pkg;
        a_pkg.name = names[i];
        a_pkg.version = versions[i];
        a_pkg.type = types[i];
        store_data(ALL_DB,&a_pkg,0);
    }
    
    printf("Print all data from test database...\n");
    get_all_data(ALL_DB);
    printf("Checking data in test database...\n");
    for (int i = 0; i < 5; i++)
    {
        struct package a_pkg;
        a_pkg.name = names[i];
        printf("Checking %s...\n",a_pkg.name);
        find_data_installed(ALL_DB,&a_pkg,NULL);
        printf("  %s => %s %s\n",a_pkg.name,a_pkg.version,a_pkg.type);
        if (strcmp(a_pkg.version,versions[i]) != 0 | strcmp(a_pkg.type,types[i]) !=0 )
        {
            msg(ERROR,"Invalid return values , database check failed");
            EXIT = -1;
        }
        
    }
    printf("Removing data from test database...\n");
    for (int i = 0; i < 5; i++)
    {
        printf("Removing %s\n",names[i]);
        remove_data(ALL_DB,names[i]);
    }
    

    printf("Checking data in test database...\n");
    for (int i = 0; i < 5; i++)
    {
        struct package a_pkg;
        a_pkg.name = names[i];
        a_pkg.version = NULL;
        a_pkg.type = NULL;
        find_data_installed(ALL_DB,&a_pkg,NULL);
        printf("  %s => %s %s\n",a_pkg.name,a_pkg.version,a_pkg.type);
        if (a_pkg.type != NULL | a_pkg.version != NULL)
        {
            msg(ERROR,"Database supression failed ");
            EXIT = -1;
        }
    }
    printf("listing all data from test database...\n");
    get_all_data(ALL_DB);

    return EXIT;
}

int test_spm()
{
    struct package o_pkg = {0};

    open_spm("tests/vim.spm",&o_pkg);

    printf("name: %s\n",o_pkg.name);
    printf("version: %s\n",o_pkg.version);
    printf("type: %s\n",o_pkg.type);

    // create a new package
    create_spm("tests/vim-test.spm",&o_pkg);

    return 0;

}

int test_ecmp(int type)
{
    init();
    int EXIT = EXIT_SUCCESS;

    printf("Testing ecmp functions\n");

    struct package pkg = {0};

    open_ecmp("tests/vim.ecmp",&pkg);
    
    // print the pkg
    printf("  %s => %s %s\n",pkg.name,pkg.version,pkg.type);

    // print package info
    printf("install script : \n");
    printf("  %s\n",pkg.info.install);
    

    // print dependencies
    printf("dependencies : %d \n",pkg.dependenciesCount);
    for (int i = 0; i < pkg.dependenciesCount; i++)
    {
        printf("  %s\n",pkg.dependencies[i]);
    }

    msg(INFO,"Creating ecmp package file");

    create_ecmp("tests/vim-test.ecmp", &pkg);

    //free_pkg(&pkg);
    

    return EXIT;
}

char* assemble(char** list,int count)
{
    char* string = calloc(256,sizeof(char));
    int i;
    for (i = 0; i < count-1; i++)
    {
        strcat(string,list[i]);
        strcat(string,",");
    }
    strcat(string,list[i]);
    return string;
}
