#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include <malloc.h>
#include <unistd.h>
#include <sys/stat.h>
#include "dirent.h"

#include "../../include/libspm.h"



#include "../../include/utils.h"

int strcpa(char** dest,const char* value)
{
    *dest = strcpy(malloc((strlen(value)+1) * sizeof(char)),value);
    return 0;
}
char** split (const char* string,char delim,int* returnCount)
{
    char* strcopy;
    strcpa(&strcopy,string);
    int count = 0;
    char *ptr = strcopy;
    while((ptr = strchr(ptr, delim)) != NULL) {
        count++;
        ptr++;
    }
    count ++;

    char** list = calloc(count+1,sizeof(char*));
    // Extract the first token
    char * token = strtok(strcopy, &delim);
    int i = 0;
    // loop through the string to extract all other tokens
    while( token != NULL ) {

        strcpa(&list[i],token);
        token = strtok(NULL, &delim);
        i++;
    }
    if (returnCount != NULL) {
        *returnCount = count;
    }

    free(strcopy);
    return list;
}
int freearr(void*** arr,unsigned long count)
{
    unsigned long i;
    for (i = 0; i < count; i++) {
        printf("Freeing string %p\n",(*arr)[i]);
        free((*arr)[i]);
    }
    printf("Freeing the array itself %p\n",*arr);
    free((*arr));
    return 0;
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
        (*buffer) = calloc(length,sizeof(char));
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
    FILE *fp;
    fp = fopen ( filePath , "wb" );
    if( !fp ) perror(filePath),exit(1);
    if(fwrite(buffer,size,1,fp) != 1) fclose(fp),fputs("entire write fails",stderr),exit(1);
    fclose(fp);
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
    int i;
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

int xis_dir (const char *d)
{
    DIR* dirptr;

    if (access ( d, F_OK ) != -1 ) {
        // file exists
        if ((dirptr = opendir (d)) != NULL) {
            closedir (dirptr); /* d exists and is a directory */
        } else {
            return -2; /* d exists but is not a directory */
        }
    } else {
        return -1;     /* d does not exist */
    }

    return 0;
}

void pmkdir (const char *dir)
{
    char tmp[256];
    char *p = NULL;
    size_t len;
    len = snprintf(tmp, sizeof(tmp),"%s",dir);
    if (tmp[len - 1] == '/')
        tmp[len - 1] = 0;
    for (p = tmp + 1; *p; p++)
        if (*p == '/') {
            *p = 0;
            mkdir(tmp, S_IRWXU);
            *p = '/';
        }
    mkdir(tmp, S_IRWXU);
}

void mvsp(char* old_path,char* new_path)
{
    // get parent dir of new_file
    int dec_count = 0;
    char** dec_path = split(new_path,'/',&dec_count);

    char* parent_dir = dec_path[dec_count-1];

    // if parent dir does not exist, create it
    if (xis_dir(parent_dir) == -1)
    {
        pmkdir(parent_dir);
    }
    else if (xis_dir(parent_dir) == -2)
    {
        msg(ERROR,"Parent dir %s is not a directory",parent_dir);
        exit(1);
    }
    else {
        msg(DBG3,"Parent dir %s exists",parent_dir);
    }

    // move file
    rename(old_path,new_path);


}




