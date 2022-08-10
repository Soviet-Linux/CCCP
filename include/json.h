#include "../lib/jsmn.h"

char* jstrval (jsmntok_t t,char* jstr);
char** jarrtoarr (jsmntok_t t_list[],char* jstr,int pos);
char* arrtojarr (char** arr,int count);
unsigned long jparse(jsmntok_t** t,char* jstr,unsigned long jsize);
