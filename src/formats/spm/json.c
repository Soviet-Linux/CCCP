#include "string.h"
#include "stdlib.h"


#include "jsmn.h"


#include "spm/utils.h"
#include "spm/libspm.h"
#include "json.h"



char* jstrval (jsmntok_t t,char* jstr)
{
    char* jstrval = format("%.*s",t.end - t.start,jstr + t.start );
    return jstrval;
}
char** jarrtoarr (jsmntok_t t_list[],char* jstr,int pos)
{
    char** arr;
    arr = calloc((t_list[pos].size) , sizeof(char*));
    int i;
    for (i = 1;i <= t_list[pos].size; i++)
    {
        msg(DBG3,"%d - %s",i,jstrval(t_list[pos+i],jstr));
        strcpa(&arr[i-1], jstrval(t_list[pos +i],jstr));
    }
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
        strcat(jarr,format("\"%s\"",arr[i]));
        strcat(jarr,",");
    }
    msg(DBG3,"Adding %s to json array",arr[i]);
    strcat(jarr,format("\"%s\"",arr[i]));
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
