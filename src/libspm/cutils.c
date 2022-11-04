#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include <malloc.h>
#include <unistd.h>
#include <sys/stat.h>
#include "dirent.h"
#include <errno.h>
#include <dlfcn.h>

#include "../../include/libspm.h"



#include "../../include/utils.h"


unsigned int splitm (char* string,char delim,char** dest,unsigned max)
{
    //msg(DBG3,"Splitting \"%s\" with delim '%c' and max %d",string,delim,max);
    char* token = strtok(string, &delim);
    unsigned int count = 0;
    while (token != NULL && count < max)
    {
        //msg(DBG3,"Token: %s",token);
        dest[count] = token;
        count++;
        token = strtok(NULL, &delim);
    }
    return count;
}



unsigned int countc(char* string,char c)
{
    unsigned int count = 0;
    for (int i = 0; i < strlen(string); i++)
    {
        if (string[i] == c)
        {
            count++;
        }
    }
    return count;
}

long rdfile(const char* filePath,char** buffer)
{
    (*buffer) = 0;
    long length;
    FILE * f = fopen (filePath, "rb");

    if (f)
    {
        fseek (f, 0, SEEK_END);
        length = ftell (f);
        fseek (f, 0, SEEK_SET);
        (*buffer) = calloc(length+1,sizeof(char));
    if ((*buffer))
    {
        fread ((*buffer), 1, length, f);
    }
    fclose (f);
    }

    if ((*buffer))
    {
        msg(DBG3,"Read %ld bytes from %s ==> %s",length,filePath,*buffer);
        return length;
    }
    else
    {
        return -1;
    }
}
int wrfile(const char* filePath,char* buffer,long size)
{
    msg(DBG3,"Writing %ld bytes to %s",size,filePath);
    FILE *fp;
    printf("fp declared\n");
    fp = fopen(filePath , "wb" );
    printf("fopen returned %p\n",fp);
    if( !fp )perror(filePath),exit(1);
    if(fwrite(buffer,size,1,fp) != 1) fclose(fp),fputs("entire write fails",stderr),exit(1);
    fclose(fp);
    printf("Writing done\n");
    return 0;
}
int strinarr( char* val, char** arr,long arrsize)
{
    int i;
    for(i = 0; i < arrsize; i++){
        //msg(DBG3,"Comparing %s with %s",val,arr[i]);
        if (strcmp(arr[i],val) == 0) return i;
    }
    return -1;
}
long findlast(char* str,char c)
{
    unsigned long i;
    for (i = strlen(str);i > 0;i--)
    {
        if (str[i] == c)
        {
            return i;
        }
    }
    return -1;
}
void popcharn(char* s,long s_size,int pos)
{
    memmove(&s[pos], &s[pos + 1], s_size - pos);
}

char** ls(char* path)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    char** list = calloc(512,sizeof(char*));
    int count = 0;
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (count > 512)
            {
                printf("Error : too many elements in list , reallocating\n");
                list = realloc(list,(count+512) * sizeof(char*));
            }
            list[count] = dir->d_name;
            count++;
        }

        closedir(d);
    }
    return list;
}


int xis_dir (const char *d)
{

    DIR* dirptr;

    if (access ( d, F_OK ) == 0 ) {
        // file exists
        if ((dirptr = opendir (d)) != NULL) {
            closedir (dirptr); /* d exists and is a directory */
            //printf("xis : Directory %s exists\n",d);
            return 0;
        } else {
            //printf("xis : Error : %s is not a directory\n",d);
            return -2; /* d exists but is not a directory */
        }
    } else {
        //printf("xis : Directory %s does not exist , exit %s\n",d,strerror(errno));
        return -1;     /* d does not exist */
    }

}

int pmkdir (const char *dir)
{
    char* parent_path = calloc(strlen(dir)+1,sizeof(char));
    strncpy(parent_path,dir,strrchr(dir, '/')-dir);

    
    printf("Parent path : %s\n",parent_path);

    // if parent dir does not exist, create it
    switch (xis_dir(parent_path))
    {
        case -1:
            if (pmkdir(parent_path) != 0) return -1;
            free(parent_path);
            return mkdir(dir,0777);
        case -2:
            msg(FATAL,"pmkdir : not a directory");
        case 0:
            free(parent_path);
            return mkdir(dir,0777); 
    }
    free(parent_path);
    return mkdir(dir,0777); 

}

int mvsp(char* old_path,char* new_path)
{
    msg(DBG3,"MVSP : Moving %s to %s",old_path,new_path);
    char* parent_path = calloc(strlen(new_path)+1,sizeof(char));
    strncpy(parent_path,new_path,strrchr(new_path, '/')-new_path);

    
    printf("Parent path : %s\n",parent_path);


    switch (xis_dir(parent_path))
    {
        case -1:
            if (pmkdir(parent_path) != 0) return -1;
            break;
        case -2:
            msg(FATAL,"mvsp : not a directory");
        case 0:
            msg(DBG3,"Prent exists , all good");
            break;
    }
    free(parent_path);
    // move file
    return rename(old_path,new_path);
}

int free_pkg(struct package* pkg)
{
    msg(DBG3,"Freeing pkg %s",pkg->name);

    free(pkg->name);
    free(pkg->version);
    free(pkg->url);
    free(pkg->license);

    free(*pkg->dependencies);
    free(pkg->dependencies);

    free(*pkg->makedependencies);
    free(pkg->makedependencies);

    free(*pkg->optionaldependencies);
    free(pkg->optionaldependencies);

    free(pkg->info.download);
    free(pkg->info.install);
    free(pkg->info.prepare);
    free(pkg->info.make);
    free(pkg->info.test);

    free(*pkg->locations);
    free(pkg->locations);



    pkg = NULL;
    return 0;
}





