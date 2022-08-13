#include "stdio.h"
#include "unistd.h"
#include "malloc.h"
#include <string.h>

#include "../../include/jsmn.h"


#define PATTERN "{\"name\":\"%s\",\"type\":\"%s\",\"version\":\"%s\",\"license\":\"%s\",\"dependencies\":[%s],\"makedependencies\":[%s],\"optionaldependencies\":[%s],\"url\":\"%s\",\"info\":{\"download\":\"%s\",\"prepare\":\"%s\",\"make\":\"%s\",\"test\":\"%s\",\"install\":\"%s\",\"special\":\"%s\"},\"locations\":[%s]}"

// class thing
#include "../../include/libspm.h"
#include "../../include/utils.h"
#include "../../include/spm.h"
#include "../../include/json.h"



char* KEYS[] = 
{
    "name",
    "type",
    "version",
    "license",
    "dependencies",
    "makedependencies",
    "optionaldependencies",
    "url",
    "info",
    "locations"

};

int KEY_COUNT = sizeof(KEYS) / sizeof(KEYS[0]);
char* INFO_KEYS[] = 
{
    "download",
    "prepare",
    "make",
    "test",
    "install",
    "special"
};
int INFO_KEY_COUNT = sizeof(INFO_KEYS) / sizeof(INFO_KEYS[0]);


int open_spm (const char* PPath,struct package* pkg)
{
    if (access(PPath, F_OK) != 0)  msg(FATAL, "PPath not found !");
    char *jstr;
    long j_size = rdfile(PPath,&jstr);
    

    // before that we need to set infos to NULL for secutiry
    pkg->info.download = NULL;
    pkg->info.prepare = NULL;
    pkg->info.make = NULL;
    pkg->info.test = NULL;
    pkg->info.install = NULL;
    pkg->info.special = NULL;


    jsmntok_t *t; /* We expect no more than 128 JSON tokens */
    unsigned long t_size  = jparse(&t,jstr,j_size);
    

    //TODO: check r

    enum EKEY {
    NAME,
    TYPE,
    VERSION,
    LICENSE,
    DEPS,
    MAKEDEPS,
    OPTDEPS,
    URL,
    INFO,
    LOCATIONS

    };
    enum INFO_EKEY {
        DOWNLOAD,
        PREPARE,
        MAKE,
        TEST,
        INSTALL,
        SPECIAL
    };

    //transform this json into a package object
    //loop over tokens
    

    // looop over json tokens
    int i;
    for (i = 0; i < t_size; i++) 
    {


        switch (t[i].type)
        {
            case JSMN_PRIMITIVE:

                break;
            case JSMN_OBJECT:
                if (
                strcmp(jstrval(t[i-1],jstr),"info") == 0 &&
                t[i].size > 0
                ) 
                {
                    for (int j  = i;j < i + t[i].size;j++)
                    {
                        int pos = strinarr(jstrval(t[i],jstr), INFO_KEYS,INFO_KEY_COUNT);
                        if (pos != -1)
                        {
                            switch (pos)
                            {
                                case DOWNLOAD:
                                    pkg->info.download = jstrval(t[j],jstr);
                                    break;
                                case PREPARE:
                                    pkg->info.prepare = jstrval(t[j],jstr);
                                    break;
                                case MAKE:
                                    pkg->info.make = jstrval(t[j],jstr);
                                    break;
                                case TEST:
                                    pkg->info.test = jstrval(t[j],jstr);
                                    break;
                                case INSTALL:
                                    pkg->info.install = jstrval(t[j],jstr);
                                    break;
                                case SPECIAL:
                                    pkg->info.special = jstrval(t[j],jstr);
                                    break;
                                
                            }
                        }
                    }
                } 


                break;
            case JSMN_ARRAY:
                

                // dependencies array

                break;
            case JSMN_STRING:
                if (t[i].size > 0)
                {
                    
                    char* j_key = jstrval(t[i],jstr);
                    

                    int pos = strinarr(j_key,KEYS,KEY_COUNT) ;

                    if (pos != -1)
                    {
                        char* j_val = jstrval(t[i+1],jstr);
                        switch (pos) 
                        {
                            case NAME:
                               pkg->name = j_val;
                                break;
                            case TYPE:
                                pkg->type= j_val;
                            case VERSION :
                                pkg->version = j_val;
                                break;
                            case LICENSE :
                                pkg->license = j_val;
                                break;
                            case URL :
                                pkg->url = j_val;
                                break;
                            default:
                                msg(FATAL, "Unknown key %s",j_key);
                                break;
                        }
                        free(j_val);
                    }
                    else {
                        // Pasing arrays :
                        // in packages , we have 4 arrays : locations, dependencies, makedependencies, optionaldependencies

                        // locations array
                        if (strcmp(KEYS[LOCATIONS],j_key) == 0)
                        {
                            if (t[i].size > 0)
                            {
                                pkg->locations = jarrtoarr(t,jstr,i);
                                pkg->locationsCount = t[i].size - 1;
                            }
                        }
                        // multiples dependencies
                        else if (strcmp(KEYS[DEPS],j_key) == 0)
                        {
                            if (t[i].size > 0)
                            {
                                pkg->dependencies = jarrtoarr(t, jstr,i);
                                pkg->dependenciesCount = t[i].size - 1;
                            }
                            else {
                                pkg->dependenciesCount = 0;
                            }
                        }
                        else if (strcmp(KEYS[OPTDEPS],j_key) == 0)
                        {
                            if (t[i].size > 0)
                            {
                                pkg->optionaldependencies = jarrtoarr(t, jstr,i);
                                pkg->optionaldependenciesCount = t[i].size - 1;
                            }
                            else {
                                pkg->optionaldependenciesCount = 0;
                            }
                        }
                        else if (strcmp(KEYS[MAKEDEPS],j_key) == 0)
                        {
                            if (t[i].size > 0)
                            {
                                pkg->makedependencies = jarrtoarr(t, jstr,i);
                                pkg->makedependenciesCount = t[i].size - 1;
                            }
                            else {
                                pkg->makedependenciesCount = 0;
                            }
                        }

                    }

                    free(j_key);
                    
                }
                break;
            case JSMN_UNDEFINED :
                printf("JSMN_UNDEFINED\n");
                break;
        }
    }    


    return 0;

    
}
int create_spm(const char* newPath,struct package* pkg)
{
    char* j_deps = arrtojarr(pkg->dependencies,pkg->dependenciesCount);
    char* j_makedeps = arrtojarr(pkg->makedependencies,pkg->makedependenciesCount);
    char* j_optdeps = arrtojarr(pkg->optionaldependencies,pkg->optionaldependenciesCount);

    char* j_locations = arrtojarr(pkg->locations,pkg->locationsCount);

    // i thought about using a json library to create the json file
    // but i found that it was too complicated to use
    // so i decided to use a simple string buffer
    char* s_json = format(
    PATTERN,// basic spm json pattern with %s to format
    pkg->name,
    pkg->type,
    pkg->version,
    pkg->license,
    pkg->url,
    j_deps,
    j_makedeps,
    j_optdeps,
    j_locations
    );

    free(j_deps);
    free(j_makedeps);
    free(j_optdeps);

    free(j_locations);

    // writing the json fil
    return wrfile(newPath,s_json,strlen(s_json));

}
// This function is very important , it will store the install location data to the "DB"


