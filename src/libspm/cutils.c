#include "string.h"
#include "stdlib.h"
#include "stdio.h"


#include "../../include/libspm.h"
#include <malloc.h>

int strcpa(char** dest,const char* value)
{
    *dest = strcpy(malloc(strlen(value) * sizeof(char)),value);
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
    int i;
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
    FILE *fp;
    long lSize;

    fp = fopen ( filePath , "rb" );
    if( !fp ) perror(filePath),exit(1);

    fseek( fp , 0L , SEEK_END);
    lSize = ftell( fp );
    rewind( fp );

    /* allocate memory for entire content */
    buffer = calloc( 1, lSize+1 );
    if( !buffer ) fclose(fp),fputs("memory alloc fails",stderr),exit(1);

    /* copy the file into the buffer */
    if(fread(*buffer,lSize,1,fp) != 1) fclose(fp),free(*buffer),fputs("entire read fails",stderr),exit(1);

    /* do your work here, buffer is a string contains the whole text */

    fclose(fp);
    return lSize;
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
        if (strcmp(arr[i],val) == 0) return i;
    }
    return -1;
}