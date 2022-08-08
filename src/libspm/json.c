#include "string.h"
#include "stdlib.h"

#include "../../lib/jsmn.h"


#include "../../include/utils.h"
#include "../../include/libspm.h"
#include "../../include/json.h"



char* jstrval (jsmntok_t t,char* jstr)
{
    char* jstrval = format("%.*s",t.end - t.start,jstr + t.start );
    return jstrval;
}
char** jarrtoarr (jsmntok_t t_list[],char* jstr,int pos)
{
    char** arr;
    arr = calloc((t_list[pos].size - 1) , sizeof(char*));
    int i;
    for (i = 1;i < t_list[pos].size; i++)
    {
        strcpa(&arr[i], jstrval(t_list[pos +i],jstr));
    }
    return arr;
}
char* arrtojarr (char** arr,int count)
{
    if (count == 0)
    {
        return "";
    }

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
    char* jarr = calloc(totsize, sizeof(char));
    int i;
    for (i=0;i < (count - 1) ;i++)
    {
        strcat(jarr,format("\"%s\"",arr[i]));
        strcat(jarr,",");
    }
    strcat(jarr,format("\"%s\"",arr[i+1]));
    return jarr;
}
