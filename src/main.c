#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <spm/libspm.h>
#include <spm/cutils.h>
#include <spm/hashtable.h>
#include <spm/globals.h>



const float C_FRONTEND_VERSION = 0.001;



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
    "          \x1b[32m -Yy,  -Nn\x1b[0m                                 Passes Y/N to promts by default   \n"
    "          \x1b[32m -i,   --install <package>\x1b[0m                 Installs a package from OUR repo   \n"
    "          \x1b[32m       --no-checksum \x1b[0m                      Bypasses cheking the hash  \n"
    "          \x1b[32m -r,   --remove <package>\x1b[0m                  Removes a package from the system    \n"
    "          \x1b[32m -l,   --list\x1b[0m                              Lists all packages installed on the system    \n"
    "          \x1b[32m -s,   --search <package>\x1b[0m                  Searches for packages that match the name provided    \n"
    "          \x1b[32m       --update\x1b[0m                            Checks if there are any updates to installed packages    \n"
    "          \x1b[32m       --upgrade\x1b[0m                           Upgrades outdated packages    \n"
    "          \x1b[32m -pkg, --package <path/to/package.ecmp>\x1b[0m    Installs a package from file provided    \n"
    "          \x1b[32m -ow,  --overwrite\x1b[0m                         Will overwrite installed packages    \n"
    "          \x1b[32m -dbg, --debug <level 0-4>\x1b[0m                 Prints debug info    \n"
    "          \x1b[32m       --verbose\x1b[0m                           Switches to verbose output    \x1b[0m \n";

int _install_source_(unsigned int* index);
int _remove_(unsigned int* index);
int _install_repo_(unsigned int* index);
int _install_repo_no_checksum_(unsigned int* index);
int _create_binary_from_file(unsigned int* i);

int _set_debug_level_(unsigned int* i);
int _set_debug_unit(unsigned int* i);
int _set_verbose_(unsigned int* i);
int _set_overwrite_(unsigned int* i);

//test
int _update_(unsigned int* i);
int _upgrade_(unsigned int* i);
int _search_(unsigned int* i);
int _set_yes_(unsigned int* i);
int _set_no_(unsigned int* i);


void* args[][2] = {
    //will test those later
    {"package",_install_source_},
    {"pkg",_install_source_},
    {"install",_install_repo_},
    {"no-checksum",_install_repo_no_checksum_},
    {"i",_install_repo_},
    {"update",_update_},
    {"upgrade",_upgrade_},
    {"remove",_remove_},
    {"r",_remove_},
    {"search",_search_},
    {"s",_search_},
    {"debug",_set_debug_level_},
    {"dbg",_set_debug_level_},
    //{"unit",_set_debug_unit},
    {"verbose", _set_verbose_},
    {"overwrite", _set_overwrite_},
    {"Yy", _set_yes_},
    {"Nn", _set_no_},
    {"ow",_set_overwrite_}
    //{"create", _create_binary_from_file}
};

char** ARGV;

int main(int argc, char *argv[]) {

    dbg(2,"DEBUG Enabled!");
    // check if not enough arguments have been passed
    if (argc == 1) {
        msg(ERROR, "No command specified add -h for information about how the command should be used.");
        return 1;
    }
    // -h and --help command
    if (0 == strcmp(argv[1], "-h") || 0 == strcmp(argv[1], "--help")) {
        printf("%s", HELP);
        return 0;
    }
    // -v and --version command
    if (0 == strcmp(argv[1], "-v") || 0 == strcmp(argv[1], "--version")){
	    printf(ART,C_FRONTEND_VERSION,version());
	    return 0;
    }
    // -l and --list command
    if (0 == strcmp(argv[1], "-l") || 0 == strcmp(argv[1], "--list")){
        readConfig("/etc/cccp.conf");
        list_installed();
	    return 0;
    }
    // root check 
    if (getuid() != 0) {
        msg(ERROR, "You need to be root to run this program for help type -h");
        return 1;
    } else {
        dbg(2,"Running as root");
    }

    hashtable* hm = hm_init(args, sizeof(args)/sizeof(args[0]));

    ARGV = argv;

    init(); 

    int (*func)(int*);
    for (int i = 1; i < argc; i++) {
        dbg(3, "argv[%d] = %s", i, argv[i]);

        // removz beginning -- in args
        if (argv[i][0] == '-' && argv[i][1] == '-') {
            argv[i] += 2;
        } else if (argv[i][0] == '-'){
            argv[i] += 1;
        }

        //hm_visualize(hm);
        // function pointer

        // get function pointer
        func = hm_get(hm, argv[i]);
        if (func == NULL) {
            msg(ERROR, "Invalid argument %s", argv[i]);
            return 1;
        }

        // call function
        func(&i);
    }


}
// install from source function
int _install_source_(unsigned int* i) {
    exit(install_package_source(ARGV[++(*i)],0));
}
// remove a pkg function
int _remove_(unsigned int* i) {
    char* pkg = ARGV[++(*i)];
    msg(INFO, "removing %s", pkg); 
    exit(uninstall(pkg));
}
// install from repo function 
int _install_repo_(unsigned int* i) {
    struct package* pkg = calloc(1, sizeof(struct package));
    pkg->name = ARGV[++(*i)];
    char* repo = get(pkg, pkg->name);

    if (repo == NULL) {
        msg(ERROR, "Failed to download package %s", pkg->name);
        return 1;
    }

    char* str = calloc(2, sizeof(char));

    msg(INFO, "Do you want to view the source for %s before installing? y/N", pkg->name);
    if(OVERWRITE_CHOISE != true)
    {
        char* res = fgets(str, 2, stdin);

        if ( strchr(str, '\n') == NULL )
        {
            while ((getchar()) != '\n');
        }

        int j = 0;

        while (str[j] != '\n' && str[j] != '\0')
        {
            j++;
        }

        if (str[j] == '\n')
        {
            str[j] = '\0';
        }
    }
    else
    {
        if(sizeof(USER_CHOISE[0]) == sizeof(str))
        {
            sprintf(str, USER_CHOISE[0]);
        }
        else
        {
            msg(FATAL, "something somwhere went wrong");
        }
    }

    if((strcmp(str, "Y") == 0 || strcmp(str, "y") == 0))
    {
        char* cmd = calloc(MAX_PATH, sizeof(char));
        sprintf(cmd, "cat %s", pkg->name);
        char* output = exec(cmd);

        printf(output);

        char* str_2 = calloc(2, sizeof(char));

        msg(INFO, "Press q to abort the installation, hit enter to continue");
        if(OVERWRITE_CHOISE != true)
        {
            char* res_2 = fgets(str_2, 2, stdin);

            if ( strchr(str_2, '\n') == NULL )
            {
                while ((getchar()) != '\n');
            }

            int k = 0;

            while (str_2[k] != '\n' && str_2[k] != '\0')
            {
                k++;
            }

            if (str_2[k] == '\n')
            {
                str_2[k] = '\0';
            }
        }
        else
        {
            if(sizeof(USER_CHOISE[0]) == sizeof(str_2))
            {
                sprintf(str_2, USER_CHOISE[0]);
            }
            else
            {
                msg(FATAL, "something somwhere went wrong");
            }
        }

        if((strcmp(str_2, "Q") == 0 || strcmp(str_2, "q") == 0))
        {
            remove(pkg->name);

            free(str);
            free(str_2);

            msg(FATAL, "Aborting...");
            return 0;
        }
        else
        {
            msg(INFO, "Continuing...");
        }

        free(str_2);
    }
    else
    {
        msg(INFO, "Continuing...");
    }

    f_install_package_source(pkg->name, 0, repo);

    remove(pkg->name);

    free(str);

    return 0;
}


// install from repo without checking for the checksum
int _install_repo_no_checksum_(unsigned int* i) {
     INSECURE = true;
     return 0;
}

int _set_debug_level_(unsigned int* i) {
    DEBUG = atoi(ARGV[++(*i)]);
    return 0;
}
int _set_debug_unit(unsigned int* i) {
    DEBUG_UNIT = ARGV[++(*i)];
    return 0;
}
int _set_verbose_(unsigned int* i) {
     QUIET = false;
     return 0;
}
int _set_overwrite_(unsigned int* i) {
    OVERWRITE = true;
    return 0;
}
// Create Bin from file function 
int _create_binary_from_file(unsigned int* i) {
    char* file = ARGV[++(*i)];
    char* binary = ARGV[++(*i)];

    create_binary_from_source(file,binary);

    return 0;
}

// update function 
int _update_(unsigned int* i)
{
    sync();
    update();
    return 0;
}
// upgrade function
int _upgrade_(unsigned int* i)
{
    upgrade();
    return 0;
}
// search function
int _search_(unsigned int* i)
{
    char* input = ARGV[++(*i)];
    search(input);
    return 0;
}

int _set_yes_(unsigned int* i) {
    OVERWRITE_CHOISE = true;
    USER_CHOISE[0] = "Y";
    return 0;
}

int _set_no_(unsigned int* i) {
    OVERWRITE_CHOISE = true;
    USER_CHOISE[0] = "N";
    return 0;
}
