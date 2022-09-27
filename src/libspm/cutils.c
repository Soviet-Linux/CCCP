#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include <malloc.h>
#include <unistd.h>
#include <sys/stat.h>
#include "dirent.h"
#include <errno.h>

#include "../../include/libspm.h"



#include "../../include/utils.h"

int strcpa(char** dest,const char* value)
{
    *dest = strcpy(malloc((strlen(value)+1) * sizeof(char)),value);
    return 0;
}
char** split (const char* string,char delim,int* returnCount)
{
    char** list = calloc(512,sizeof(char*));
    char* buffer = calloc(256,sizeof(char));

    char* ptr = string;

    int count = 0;
    while ((*ptr) != '\0')
    {
        
        if (*ptr == delim)
        {
            if (count > 512)
            {
                printf("Error : too many elements in list , reallocating\n");
                list = realloc(list,(count+512) * sizeof(char*));
            }

            list[count] = buffer;
            buffer = calloc(256,sizeof(char));
            count++;
        }
        else
        {
            buffer[strlen(buffer)] = *ptr;
        }
        ptr++;
        

    }
    list[count++] = buffer;



    *returnCount = count;
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
            printf("xis : Directory %s exists\n",d);
            return 0;
        } else {
            printf("xis : Error : %s is not a directory\n",d);
            return -2; /* d exists but is not a directory */
        }
    } else {
        printf("xis : Directory %s does not exist , exit %s\n",d,strerror(errno));
        return -1;     /* d does not exist */
    }

}

int pmkdir (const char *dir)
{
    char* parent_path = calloc(256,sizeof(char));

    strcpa(&parent_path,dir);

    char* parent_pos = strrchr(parent_path, '/');
    if (parent_pos == NULL)
    {
        printf("Parent path is %s , errorr\n",parent_path);
        return -1;
    }
    else
    {
        parent_pos[0] = '\0';
        // print parent path hex
        //printf("Parent path is %s\n",parent_path);

    }
    printf("Parent path is %s\n",parent_path);

    int xis = xis_dir(parent_path);

    // if parent dir does not exist, create it
    if (xis == -1)
    {
        msg(DBG3,"Parent dir %s does not exist, creating it",parent_path);
        pmkdir(parent_path);
        // check if mkdir success
        if (xis_dir(parent_path) == -1)
        {
            msg(DBG3,"Error : could not create parent dir %s",parent_path);
            // print errno
            printf("Error : %s\n",strerror(errno));
            return -1;
        }
    }
    else if (xis == -2)
    {
        msg(DBG3,"Parent dir %s exists but is not a directory",parent_path);
        msg(ERROR,"Parent dir %s is not a directory",parent_path);
        exit(1);
    }
    else {
        msg(DBG3,"Parent dir %s exists creating dir",parent_path);
        return mkdir(dir,0777);

        
    }

}

void mvsp(char* old_path,char* new_path)
{
    msg(DBG3,"MVSP : Moving %s to %s",old_path,new_path);
    char* parent_path = calloc(256,sizeof(char));
    strcpa(&parent_path,new_path);

    char* parent_pos = strrchr(parent_path, '/');
    if (parent_pos == NULL)
    {
        printf("Parent path is %s , errorr\n",parent_path);
        return;
    }
    else
    {
        parent_pos[0] = '\0';
        // print parent path hex
        //printf("Parent path is %s\n",parent_path);

    }
    printf("Parent path is %s\n",parent_path);

    int xis = xis_dir(parent_path);

    // if parent dir does not exist, create it
    if (xis == -1)
    {
        msg(DBG3,"Parent dir %s does not exist, creating it",parent_path);
        mkdir(parent_path,0777);
        // check if mkdir success
        if (xis_dir(parent_path) == -1)
        {
            msg(DBG3,"Error : could not create parent dir %s",parent_path);
            // print errno
            printf("Error : %s\n",strerror(errno));
            return;
        }
    }
    else if (xis == -2)
    {
        msg(DBG3,"Parent dir %s exists but is not a directory",parent_path);
        msg(ERROR,"Parent dir %s is not a directory",parent_path);
        exit(1);
    }
    else {
        msg(DBG3,"Parent dir %s exists",parent_path);
    }

    // move file
    if (rename(old_path,new_path) == 0)
    {
        msg(DBG3,"Moved %s to %s",old_path,new_path);
    }
    else
    {
        msg(ERROR,"Could not move %s to %s error %s",old_path,new_path,strerror(errno));
        exit(1);
    }


}




