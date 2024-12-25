#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <spm/libspm.h>
#include <spm/cutils.h>
#include <spm/hashtable.h>
#include <spm/globals.h>

const float C_FRONTEND_VERSION = 1.001;
// This is very stupid, but I don't want to work on this right now.
#define QUEUE_MAX 65536

struct packages* install_queue;
struct packages* update_queue;
struct packages* remove_queue;

char* ART = "\033[31;1;1m"
    "    ⠀⠀⠀⠀⠀⠀⠀⠀⣇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n"
    "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡼⢹⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
    "⠀⠀⠀⠀⠀⠀⠀⠙⠿⣟⠛⠃⠀⠛⢛⡿⠟⠁⠀⠀⠀⠀⠀⠀       |============================|\n"
    "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⡗⠀⡀⠐⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
    "⠀⠀⠀⣀⣴⠊⠀⠀⠀⣸⡵⠋⠉⠳⣽⡀⠀⠀⠙⢦⣄⡀⠀⠀          CCCP C Frontend Version : \n"
    "⠀⢠⣾⣼⠕⠀⠀⠀⠀⠁⠀⠀⠠⣀⠈⠁⠀⠀⠀⠹⣗⢳⢆⠀                      %.3f\n"
    "⠀⣧⣟⡕⠀⠀⠀⠀⠀⢀⣤⡤⠀⠈⠻⣦⡀⠀⠀⠀⢹⣿⢸⡀\n"
    "⢸⢻⣿⠀⠀⠀⠀⠀⣴⣿⢿⡀⠀⠀⠀⠘⣿⡄⠀⠀⠀⡿⡾⣠\n"
    "⢸⣎⡾⡀⠀⠀⠀⠀⠈⠁⠀⠙⣦⣄⠀⠀⣿⡇⠀⠀⠀⣽⣴⣇          C LibSPM Version : \n"
    "⢰⣝⣇⡇⠀⠀⠀⠀⣠⣄⣀⠀⠈⠻⣷⣼⣿⠃⠀⠀⢀⡏⢏⡞                      %.3f\n"
    "⠀⢝⢮⣇⣆⠀⢠⣶⠋⠈⠙⠻⠿⠿⠿⠿⣷⡄⠀⢰⢻⡷⣫⠀\n"
    "⠀⠈⠳⣯⣽⡖⣌⠁⠀⠀⠀⠀⠀⠀⠀⠀⠈⣠⢾⣟⣽⡞⠁⠀       |============================|\n"
    "⠀⠀⠀⠑⠯⣭⣼⡷⠤⣄⡠⢄⣠⢤⣀⡤⠾⣯⣭⡽⠊⠀⠀⠀\n"
    "⠀⠀⠀⠀⠀⠑⠒⠚⠛⣩⠞⠁⠀⠙⢮⠛⠛⠒⠊⠀⠀⠀⠀⠀\n"
    "\033[0m";

char* HELP = "\x1b[34m Usage cccp [options/package] (Options are evaluated in order form left to right)\x1b[0m \n"
    "\x1b[32m Options:  -v,   --version \x1b[0m                          Displays the version \n"
    "          \x1b[32m -h,   --help\x1b[0m                              Displays this message   \n"
    "          \x1b[32m -i,   --install <package>\x1b[0m                 Install a package   \n"
    "          \x1b[32m -r,   --remove <package>\x1b[0m                  Remove a package    \n"
    "          \x1b[32m -l,   --list\x1b[0m                              List all packages installed on the system    \n"
    "          \x1b[32m -s,   --search <package>\x1b[0m                  Search for packages    \n"
    "          \x1b[32m -u,   --update\x1b[0m                            Update installed packages    \n"
    "          \x1b[32m -a,   --auto\x1b[0m                              Use defaults and ignore prompts   \n"
    "          \x1b[32m -c,   --clean\x1b[0m                             Clean up the cache directory    \n"
    "          \x1b[32m -pkg, --package <path/to/package.ecmp>\x1b[0m    Install a package from file    \n"
    "          \x1b[32m -dbg, --debug <level 0-4>\x1b[0m                 Print debug info    \n"
    "          \x1b[32m -q,   --quiet           \x1b[0m                  Ignore make output    \n";

int _install_source_(unsigned int* index);
int _install_repo_(unsigned int* index);
int _remove_(unsigned int* index);
int _set_debug_level_(unsigned int* i);     // works
int _set_auto_(unsigned int* i);            // works
int _set_quiet_(unsigned int* i);           // works
int _clean_up_(unsigned int* i);            // works
int _update_(unsigned int* i);              // works
int _search_(unsigned int* i);              // works

int add_to_queue(struct package* pkg, struct packages* queue);
void ask_to_preview_pkg(char* path);
void populate_installed_db();               // works
void populate_remote_db();                  // works
int check_optional_dependencies(char **dependencies, int dependenciesCount);
int check_dependencies(char **dependencies, int dependenciesCount);

void* args[][2] = 
{
    {"--package"  , _install_source_   },
    {"-pkg"      , _install_source_    },
    {"--install"  , _install_repo_     },
    {"-i"        , _install_repo_      },
    {"--update"   , _update_           },
    {"-u"        , _update_            },
    {"--remove"   , _remove_           },
    {"-r"        , _remove_            },
    {"--search"   , _search_           },
    {"-s"        , _search_            },
    {"--debug"    , _set_debug_level_  },
    {"-dbg"      , _set_debug_level_   },
    {"--auto"     , _set_auto_         },
    {"-a"        , _set_auto_          },
    {"--clean"    , _clean_up_         },
    {"-c"        , _clean_up_          },
    {"--quite"    , _set_quiet_        },
    {"-q"        , _set_quiet_         },
};

char** ARGV;
int ARGC;

int main(int argc, char *argv[]) 
{
    dbg(2,"DEBUG Enabled!");
    // check if not enough arguments have been passed
    if (argc == 1) 
    {
        msg(ERROR, "No command specified add -h for information about how the command should be used.");
        return 1;
    }
    // -h and --help command
    if (0 == strcmp(argv[1], "-h") || 0 == strcmp(argv[1], "--help")) 
    {
        printf("%s", HELP);
        return 0;
    }
    // -v and --version command
    if (0 == strcmp(argv[1], "-v") || 0 == strcmp(argv[1], "--version"))
    {
	    printf(ART,C_FRONTEND_VERSION,version());
	    return 0;
    }
    // -l and --list command
    if (0 == strcmp(argv[1], "-l") || 0 == strcmp(argv[1], "--list"))
    {
        readConfig("/etc/cccp.conf", 0);
        struct packages* installed_packages = dump_db(getenv("SOVIET_INSTALLED_DB"));
        if(installed_packages->count > 0)
        {
            for(int i = 0; i < installed_packages->count; i++)
            {
                printf("%s\n", installed_packages->buffer[i].name);
            }
        }
        free_pkgs(installed_packages);
	    return 0;
    }
    // root check 
    if (getuid() != 0) 
    {
        // In the future, we might want to just set SOVIET_ROOT to SOVIET_USER_ROOT and continue
        msg(ERROR, "You need to be root to run this program for help type -h");
        return 1;
    } 
    else 
    {
        dbg(2,"Running as root");
    }

    install_queue = create_pkgs(0);
    update_queue  = create_pkgs(0);
    remove_queue  = create_pkgs(0);

    hashtable* hm = hm_init(args, sizeof(args)/sizeof(args[0]));

    ARGV = argv;
    ARGC = argc;

    init(); 

    int (*func)(int*);
    for (int i = 1; i < argc; i++) 
    {
        dbg(3, "argv[%d] = %s", i, argv[i]);

        //hm_visualize(hm);
        // function pointer

        // get function pointer
        func = hm_get(hm, argv[i]);
        if (func == NULL) 
        {
            msg(ERROR, "Invalid argument %s", argv[i]);
            return 1;
        }

        // call function
        func(&i);
    }
    hm_destroy(hm);

    if((install_queue->count != 0) || (update_queue->count != 0) || (remove_queue->count != 0))
    {
        msg(INFO, "INSTALL QUEUE:");
        for(int iq = 0; iq < install_queue->count; iq++)
        {
            printf("%d - %s\n", iq, install_queue->buffer[iq].name);
        }

        msg(INFO, "UPDATE QUEUE:");
        for(int uq = 0; uq < update_queue->count; uq++)
        {
            printf("%d - %s\n", uq, update_queue->buffer[uq].name);
        }

        msg(INFO, "REMOVE QUEUE:");
        for(int rq = 0; rq < remove_queue->count; rq++)
        {
            printf("%d - %s\n", rq, remove_queue->buffer[rq].name);
        }

        printf("writing changes to the database...\n");
        populate_installed_db();
        printf("done.\n");
    }

    free_pkgs(install_queue);
    free_pkgs(update_queue);
    free_pkgs(remove_queue);

}

// install from source function
int _install_source_(unsigned int* i) 
{
    // Get the name and search it in the database
    char* path = ARGV[++(*i)];

    char* pkg_path = path;
    struct package pkg = {0};
    pkg.path = strdup(path);
    pkg.name = strdup(path);
    open_pkg(pkg_path, &pkg);
    add_to_queue(&pkg, install_queue);
    return 0;
}

// remove a pkg function
int _remove_(unsigned int* i) 
{
    // While not next argument
    while((*i) < ARGC - 1)
    {
        if (ARGV[(*i) + 1][0] != '-')
        {
            // Get the name and search it in the database
            char* name = ARGV[++(*i)];
            struct packages* pkgs = search_pkgs(getenv("SOVIET_INSTALLED_DB"), name);
            if(pkgs->count == 0)
            {
                printf("Could not find '%s' in installed packages\n", name);
            }
            else
            {
                for(int i = 0; pkgs->count > 0; i++)
                {
                    struct package* pkg = pop_pkg(pkgs);
                    if(strcmp(pkg->name, name) == 0)
                    {
                        printf("Uninstall ");
                        if(get_input(pkg->name, 1))
                        {
                            open_pkg(getenv("SOVIET_SPM_DIR"), pkg);
                            add_to_queue(pkg, remove_queue);
                            break;
                        }
                        else
                        {
                            printf("skipping…\n"); 
                            free_pkg(pkg);
                        }
                    }
                    else
                    {
                        free_pkg(pkg);
                        if(pkgs->count == 0)
                        {
                            printf("Could not find '%s' in installed packages\n", name);
                        }
                    }
                }
            }
            free_pkgs(pkgs);
        }
        else
        {
            return 0;
        }
    }
    return 0;
}

// install from repo function 
int _install_repo_(unsigned int* i) 
{
    // While not next argument
    while((*i) < ARGC - 1)
    {
        if (ARGV[(*i) + 1][0] != '-')
        {
            // Get the name and search it in the database
            char* name = ARGV[++(*i)];
            struct packages* pkgs = search_pkgs(getenv("SOVIET_ALL_DB"), name);
            
            if(pkgs->count == 0)
            {
                printf("0 matches for '%s'\n", name);
            }
            else
            {
                for(int i = 0; pkgs->count > 0; i++)
                {
                    struct package* pkg = pop_pkg(pkgs);
                    if(strcmp(pkg->name, name) == 0)
                    {
                        printf("Install ");
                        if(get_input(pkg->name, 1))
                        {
                            open_pkg(getenv("SOVIET_REPOS_DIR"), pkg);
                            if(check(pkg) == 0)
                            {
                                printf("package %s is already installed\n", pkg->name);
                                break;
                            }
                            add_to_queue(pkg, install_queue);
                            break;
                        }
                        else
                        {
                            printf("skipping…\n"); 
                            free_pkg(pkg);
                        }
                    }
                    else
                    {
                        free_pkg(pkg);
                        if(pkgs->count == 0)
                        {
                            printf("Could not find '%s', try searching with cccp -s '%s'\n", name, name);
                        }
                    }
                }
            }
            free_pkgs(pkgs);
        }
        else
        {
            return 0;
        }
    }
    return 0;
}

// update function 
int _update_(unsigned int* i)
{
    repo_sync();
    struct packages* need_updating = update_pkg();
    if(need_updating->count > 0)
    {
        for(int i = 0; need_updating->count > 0; i++)
        {
            struct package* pkg = pop_pkg(need_updating);
            printf("Update ");
            if(get_input(pkg->name, 1))
            {
                open_pkg(getenv("SOVIET_REPOS_DIR"), pkg);
                // In the future, the package should be added to the queue, so cccp can continue parsing flags
                // The queue can be installed at the end of the main function
                add_to_queue(pkg, update_queue);
            }
            else
            {
                printf("skipping…\n"); 
                free_pkg(pkg);
            }
        }
    }
    free_pkgs(need_updating);
    populate_remote_db();
    return 0;
}

// search function
int _search_(unsigned int* i)
{
    // Get the name and search it in the database
    char* name = ARGV[++(*i)];
    struct packages* pkgs = search_pkgs(getenv("SOVIET_ALL_DB"), name);
    for(int i = 0; pkgs->count > 0; i++)
    {
        struct package* pkg = pop_pkg(pkgs);
        printf("%s\n", pkg->name);
        free_pkg(pkg);
    }
    free_pkgs(pkgs);
}

// cleanup function
int _clean_up_(unsigned int* i)
{
    if(getenv("SOVIET_SOURCE_DIR") != NULL)
    {
        rmrf(getenv("SOVIET_SOURCE_DIR"));
        return 0;
    } 
    else
    {
        msg(FATAL, "No source directory exists");
    }

    return 0;
}

// flags
int _set_debug_level_(unsigned int* i)
{
    DEBUG = atoi(ARGV[++(*i)]);
    return 0;
}

int _set_quiet_(unsigned int* i) 
{
    QUIET = true;
    return 0;
}

int _set_auto_(unsigned int* i) 
{
    AUTO = true;
    return 0;
}

// helpful functions
void ask_to_preview_pkg(char* path) 
{
    if(get_input("View package file ", 0))
    {
        FILE *fptr; 
    
        // Open file 
        fptr = fopen(path, "r"); 
        if (fptr == NULL) 
        { 
            printf("Cannot open file \n"); 
            exit(0); 
        } 
    
        // Read contents from file 
        char c = fgetc(fptr); 
        while (c != EOF) 
        { 
            printf ("%c", c); 
            c = fgetc(fptr); 
        } 
    
        fclose(fptr); 
        
        if(!get_input("Continue", 1))
        {
            msg(FATAL, "Aborting...");
        }
    }
}

int add_to_queue(struct package* pkg, struct packages* queue)
{
    if(pkg->dependenciesCount > 0)
    {
        check_dependencies(pkg->dependencies, pkg->dependenciesCount);
    }    
    if(pkg->optionalCount > 0)
    {
        check_optional_dependencies(pkg->optional, pkg->optionalCount);
    }
    dbg(2, "%s added to queue", pkg->name);
    push_pkg(queue, pkg);
    if(queue->count > QUEUE_MAX)
    {
        msg(FATAL, "dependency tree too large"); /*or you just decided to update/remove 65k packages*/
    }
    return 0;
}

void populate_installed_db()
{
    rmany(getenv("SOVIET_INSTALLED_DB"));
    struct packages* installed = get_pkgs(getenv("SOVIET_SPM_DIR"));
    create_pkg_db(getenv("SOVIET_INSTALLED_DB"), installed);
    free_pkgs(installed);
}

void populate_remote_db()
{
    rmany(getenv("SOVIET_ALL_DB"));
    struct packages* remote = get_pkgs(getenv("SOVIET_REPOS_DIR"));
    create_pkg_db(getenv("SOVIET_ALL_DB"), remote);
    free_pkgs(remote);
}


// Function to check if all dependencies of a package are installed
int check_dependencies(char **dependencies, int dependenciesCount) 
{
    dbg(1, "Checking dependencies...");

    for (int i = 0; i < dependenciesCount; i++) 
    {
        // Get the name and search it in the database
        // It's kinda stupid
        int is_installed = 0;
        char* name = dependencies[i];
        struct packages* pkgs = search_pkgs(getenv("SOVIET_INSTALLED_DB"), name);
        if(pkgs->count != 0)
        {
            for(int j = 0; pkgs->count > 0; j++)
            {
                struct package* pkg = pop_pkg(pkgs);
                if(strcmp(pkg->name, name) == 0)
                {
                    free_pkg(pkg);
                    dbg(2, "dependency %s is installed", name);
                    is_installed = 1;
                    break;
                }
                else
                {
                    if(pkgs->count == 0)
                    {
                        free_pkg(pkg);
                        break;
                    }
                    free_pkg(pkg);
                }
            }
        }
        if(!is_installed)
        {
            dbg(2, "dependency %s is missing", name);

            // Get the name and search it in the database
            free_pkgs(pkgs);
            pkgs = search_pkgs(getenv("SOVIET_ALL_DB"), name);
            
            if(pkgs->count == 0)
            {
                printf("Could not resolve dependency '%s'\n", name);
            }
            else
            {
                for(int j = 0; pkgs->count > 0; j++)
                {
                    struct package* pkg = pop_pkg(pkgs);
                    if(strcmp(pkg->name, name) == 0)
                    {
                        open_pkg(getenv("SOVIET_REPOS_DIR"), pkg);
                        add_to_queue(pkg, install_queue);
                        break;
                    }
                    else
                    {
                        free_pkg(pkg);
                        if(pkgs->count == 0)
                        {
                            printf("Could not resolve dependency '%s'\n", name);
                            break;
                        }
                    }
                }
            }
        }

        free_pkgs(pkgs);
    }
    return 0;
}

// Function to check if all optional dependencies of a package are installed
int check_optional_dependencies(char **dependencies, int dependenciesCount) 
{
    dbg(1, "Checking dependencies...");

    for (int i = 0; i < dependenciesCount; i++) 
    {
        // Get the name and search it in the database
        // It's kinda stupid
        int is_installed = 0;
        char* name = dependencies[i];
        struct packages* pkgs = search_pkgs(getenv("SOVIET_INSTALLED_DB"), name);
        if(pkgs->count != 0)
        {
            for(int j = 0; pkgs->count > 0; j++)
            {
                struct package* pkg = pop_pkg(pkgs);
                if(strcmp(pkg->name, name) == 0)
                {
                    free_pkg(pkg);
                    dbg(2, "dependency %s is installed", name);
                    is_installed = 1;
                    break;
                }
                else
                {
                    if(pkgs->count == 0)
                    {
                        free_pkg(pkg);
                        break;
                    }
                    free_pkg(pkg);
                }
            }
        }

        if(!is_installed)
        {
            dbg(2, "dependency %s is missing", name);

            // Get the name and search it in the database
            free_pkgs(pkgs);
            pkgs = search_pkgs(getenv("SOVIET_ALL_DB"), name);
            
            if(pkgs->count == 0)
            {
                printf("Could not resolve dependency '%s'\n", name);
            }
            else
            {
                for(int j = 0; pkgs->count > 0; j++)
                {
                    struct package* pkg = pop_pkg(pkgs);
                    if(strcmp(pkg->name, name) == 0)
                    {
                        printf("Install ");
                        if(get_input(pkg->name, 1))
                        {
                            open_pkg(getenv("SOVIET_REPOS_DIR"), pkg);
                            add_to_queue(pkg, install_queue);
                            break;
                        }
                        else
                        {
                            printf("skipping…\n"); 
                            free_pkg(pkg);
                            break;
                        }
                    }
                    else
                    {
                        free_pkg(pkg);
                        if(pkgs->count == 0)
                        {
                            printf("Could not resolve dependency '%s'\n", name);
                            break;
                        }
                    }
                }
            }
        }

        free_pkgs(pkgs);
    }
    return 0;
}