#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include <malloc.h>

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
    printf("String contains %d words\n", count);
    char** list = malloc((sizeof(char*)* count) + sizeof(void*));
    printf("Allocated %zu bytes for %d string pointers\n",malloc_usable_size(list),count);
    // Extract the first token
    char * token = strtok(strcopy, &delim);
    int i = 0;
    // loop through the string to extract all other tokens
    while( token != NULL ) {
        printf("%d - %s \n",i,token);
        strcpa(&list[i],token);
        printf("list[%d] = %p and value is %s\n",i,list[i],list[i]);
        token = strtok(NULL, &delim);
        i++;
    }
    printf("After while\n");
    printf("list = %p\n",list);
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

long dynappend(char*** arr,unsigned int len,char* s)
{
    unsigned int s_len = strlen(s);
    *arr = realloc(*arr, len + ( s_len * sizeof(char)));

    return s_len + len;
}

int sfree(char** ptr)
{
    if (strlen(*ptr) != 0)
    {
        free(*ptr);
        return 0;
    }
    else {
        msg(DBG3,"No free because string is null");
        return 1;
    }
}
    