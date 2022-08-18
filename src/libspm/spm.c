#include "stdio.h"
#include "unistd.h"
#include "malloc.h"
#include <string.h>

#include "../../include/jsmn.h"


#define PATTERN "{\"name\":\"%s\",\"type\":\"%s\",\"version\":\"%s\",\"license\":\"%s\",\"url\":\"%s\",\"dependencies\":[%s],\"makedependencies\":[%s],\"optionaldependencies\":[%s],\"info\":{\"download\":\"%s\",\"prepare\":\"%s\",\"make\":\"%s\",\"test\":\"%s\",\"install\":\"%s\",\"special\":\"%s\"},\"locations\":[%s]}"

// class thing
#include "../../include/libspm.h"
#include "../../include/utils.h"
#include "../../include/spm.h"
#include "../../include/json.h"






int open_spm (const char* PPath,struct package* pkg)
{
    if (access(PPath, F_OK) != 0)  msg(FATAL, "PPath not found !");
    char *jstr;
    msg(DBG2,"Reading file");
    long j_size = rdfile(PPath,&jstr);
    
    msg(DBG2,"Setting everything to NULL"); 



    //set all varibales t NULL
    pkg->name = NULL;
    pkg->type = NULL;
    pkg->version = NULL;
    pkg->license = NULL;
    pkg->dependencies = NULL;
    pkg->makedependencies = NULL;
    pkg->optionaldependencies = NULL;
    pkg->url = NULL;
    pkg->locations = NULL;
    pkg->locationsCount = 0;
    pkg->info.download = NULL;
    pkg->info.prepare = NULL;
    pkg->info.make = NULL;
    pkg->info.test = NULL;
    pkg->info.install = NULL;
    pkg->info.special = NULL;
    pkg->info.download = NULL;
    pkg->info.prepare = NULL;




    msg(DBG3,"Parsing json : %s",jstr);
    jsmntok_t *t; /* We expect no more than 128 JSON tokens */
    unsigned long t_size  = jparse(&t,jstr,j_size);
    

    //TODO: check r
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
    
    msg(DBG2,"Looping over tokens %d",t_size);
    // looop over json tokens
    int i;
    for (i = 0; i < t_size; i++) 
    {

        switch (t[i].type)
        {
            case JSMN_PRIMITIVE:

                break;
            case JSMN_OBJECT:
                //msg(DBG3,"Object key : %s",jstrval(t[i-1],jstr));
                if 
                (
                 strcmp(jstrval(t[i-1],jstr),"info") == 0 &&
                 t[i].size > 0
                ) 
                {
                    
                    for (int j  = i+1;j < i + (t[i].size * 2);j++)
                    {
                        //msg(DBG3,"Info key : %s",jstrval(t[j],jstr));
                        int pos = strinarr(jstrval(t[j],jstr), INFO_KEYS,INFO_KEY_COUNT);
                        if (pos != -1)
                        {
                            switch (pos)
                            {
                                case DOWNLOAD:
                                    pkg->info.download = jstrval(t[j+1],jstr);
                                    break;
                                case PREPARE:
                                    pkg->info.prepare = jstrval(t[j+1],jstr);
                                    break;
                                case MAKE:
                                    pkg->info.make = jstrval(t[j+1],jstr);
                                    break;
                                case TEST:
                                    pkg->info.test = jstrval(t[j+1],jstr);
                                    break;
                                case INSTALL:
                                    pkg->info.install = jstrval(t[j+1],jstr);
                                    break;
                                case SPECIAL:
                                    pkg->info.special = jstrval(t[j+1],jstr);
                                    break;
                                default:
                                    msg(WARNING,"Unknown info key : %s",jstrval(t[j],jstr));
                                    break;
                                
                            }
                        }
                        else {
                            //msg(WARNING,"No info keys found");
                        } 
                    }
                    
                    i += (t[i].size * 2);
                    
                } 


                break;
            case JSMN_ARRAY:


                break;
            case JSMN_STRING:
                if (t[i].size > 0)
                {
                    
                    char* j_key = jstrval(t[i],jstr);
                    
                    //msg(DBG3,"Key: %s",j_key);

                    int pos = strinarr(j_key,KEYS,KEY_COUNT) ;

                    if (pos != -1)
                    {
                        char* j_val = jstrval(t[i+1],jstr);
                        switch (pos) 
                        {
                            case NAME:
                                pkg->name = j_val;
                                //msg(DBG3,"Name is %s",pkg->name);
                                i++;
                                break;
                            case TYPE:
                               // msg(DBG3,"Type: %s",j_val);
                                pkg->type= j_val;
                                i++;
                                break;
                            case VERSION :
                                //msg(DBG3,"Version is %s",j_val);
                                pkg->version = j_val;
                                i++;
                                break;
                            case LICENSE :
                                pkg->license = j_val;
                                i++;
                                break;
                            case URL :
                                pkg->url = j_val;
                                i++;
                                break;
                            case DEPS :
                                if (t[i+1].size > 0)
                                {
                                    msg(DBG3,"Converting %s to array of size %d",jstrval(t[i+1],jstr),t[i+1].size);
                                    pkg->dependencies = jarrtoarr(t, jstr,i+1);
                                    pkg->dependenciesCount = t[i+1].size;
                                }
                                else {
                                    pkg->dependenciesCount = 0;
                                }
                                break;
                            case MAKEDEPS :
                                if (t[i+1].size > 0)
                                {
                                    msg(DBG3,"Converting %s to array of size %d",jstrval(t[i+1],jstr),t[i+1].size);
                                    pkg->makedependencies = jarrtoarr(t, jstr,i+1);
                                    pkg->makedependenciesCount = t[i+1].size;
                                }
                                else {
                                    pkg->makedependenciesCount = 0;
                                }
                                break;
                            case OPTDEPS :
                                if (t[i+1].size > 0)
                                {
                                    msg(DBG3,"Converting %s to array of size %d",jstrval(t[i+1],jstr),t[i+1].size);
                                    pkg->optionaldependencies = jarrtoarr(t, jstr,i+1);
                                    pkg->optionaldependenciesCount = t[i+1].size;
                                }
                                else {
                                    pkg->optionaldependenciesCount = 0;
                                }
                                break;
                            case LOCATIONS:
                                if (t[i].size > 0)
                                {
                                    pkg->locations = jarrtoarr(t,jstr,i);
                                    pkg->locationsCount = t[i].size - 1;
                                }

                            default:
                                //msg(FATAL, "Unknown key %s",j_key);
                                free(j_val);
                                break;
                        }
                        //free(j_val);
                    }
                    else {
                        msg(DBG3,"Unknown key %s",j_key);
                    }

                    free(j_key);
                    
                }
                break;
            case JSMN_UNDEFINED :
                printf("JSMN_UNDEFINED\n");
                break;
        }
    }    



    //print all dependencies

    msg(DBG3,"Dependencies :");
    for (i = 0; i < pkg->dependenciesCount; i++)
    {
        msg(DBG3,"%s",pkg->dependencies[i]);
    }


    return 0;

    
}
int create_spm(const char* newPath,struct package* pkg)
{
    msg(DBG3,"Creating spm at %s",newPath);

    msg(DBG3,"dpendencies = %s",pkg->dependencies[0]);

    char* j_deps = arrtojarr(pkg->dependencies,pkg->dependenciesCount);
    printf("stage 1\n");
    char* j_makedeps = arrtojarr(pkg->makedependencies,pkg->makedependenciesCount);
    printf("stage 2\n");
    char* j_optdeps = arrtojarr(pkg->optionaldependencies,pkg->optionaldependenciesCount);
    printf("stage 3\n");
    char* j_locations = arrtojarr(pkg->locations,pkg->locationsCount);

    printf("cretaing json\n");
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
    pkg->info.download,
    pkg->info.prepare,
    pkg->info.make,
    pkg->info.test,
    pkg->info.install,
    pkg->info.special,
    j_locations
    
    );
    msg(DBG3,"Json created! : %s\n",s_json);

    free(j_deps);
    free(j_makedeps);
    free(j_optdeps);

    free(j_locations);

    // writing the json fil
    return wrfile(newPath,s_json,strlen(s_json));

}
// This function is very important , it will store the install location data to the "DB"


