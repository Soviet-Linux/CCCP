#include "string.h"
#include "stdlib.h"


#include "jsmn.h"


#include "spm/utils.h"
#include "spm/libspm.h"
#include "json.h"
#include <stdio.h>
#include <stdlib.h>



char* jstrval (jsmntok_t t,char* jstr)
{
    char* jstrval = calloc(t.end-t.start+1,sizeof(char)); 
    sprintf(jstrval,"%.*s",t.end - t.start,jstr + t.start );
    return jstrval;
}
char** jarrtoarr (jsmntok_t t_list[],char* jstr,int pos)
{
    // copy the json string array
    char* jarr = jstrval(t_list[pos],jstr);
    // remove the brackets
    jarr = jarr+1;
    jarr[strlen(jarr)-1] = '\0';
    // remove the ""
    for (int i = 0; i < strlen(jarr); i++)
    {
        if (jarr[i] == '"')
        {
            popcharn(jarr,strlen(jarr),i);
        }
    }
    // split
    char** arr = calloc(t_list[pos].size,sizeof(char*));
    splitm(jarr,',',arr,t_list[pos].size);
    return arr;
}
char* arrtojarr (char** arr,int count)
{
    msg(DBG3,"Converting array of %d to json array",count);
    if (count == 0)
    {
        msg(DBG3,"Returning empty value");
        char* empty = calloc(1, sizeof(char));
        empty[0] = '\0';
        return empty;
    }
    msg(DBG3,"Calculating size of json array");
    unsigned long totsize = 0;
    for (int j =0;j < count;j++)
    {
        // ading size of ach string
        totsize += strlen(arr[j]);
        // adding size of comma
        totsize += 1;
        // adding zize of quotes
        totsize += 2;


    }
    msg(DBG3,"Allocating memory for json array");
    char* jarr = calloc(totsize, sizeof(char));
    int i;
    for (i=0;i < (count - 1) ;i++)
    {
        msg(DBG3,"Adding %s to json array",arr[i]);
        strcat(jarr,"\"");
        strcat(jarr,arr[i]);
        strcat(jarr,"\",");
    }
    msg(DBG3,"Adding %s to json array",arr[i]);
    strcat(jarr,"\"");
    strcat(jarr,arr[i]);
    strcat(jarr,"\"");

    return jarr;
}
unsigned long jparse(jsmntok_t** t,char* jstr,unsigned long jsize)
{
    jsmn_parser p;
    jsmn_init(&p);

    unsigned long t_num = jsmn_parse(&p, jstr, jsize, NULL,2048);
    jsmn_init(&p);


    *t = malloc(t_num * sizeof(jsmntok_t));

    jsmn_parse(&p, jstr, jsize, *t, t_num);
    msg(DBG3,"Parsed %d tokens",t_num);
    return p.toknext;
}


/*
["a","b","c"]
*/

