#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include </usr/include/spm/libspm.h>
#include </usr/include/spm/cutils.h>
#include </usr/include/spm/hashtable.h>
#include </usr/include/spm/globals.h>


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

char* HELP = "\x1b[34m Usage cccp [options/package] \x1b[0m \n"
    "\x1b[32m Options: 1) --version\x1b[0m Displays the version \n"
    "          \x1b[32m2) --help\x1b[0m Displays this message   \n"
    "          \x1b[32m3) --install <package>\x1b[0m Installs a package from OUR reopo    \n"
    "          \x1b[32m4) --remove <package>\x1b[0m Removes a package from the system    \n"
    "          \x1b[32m5) --list\x1b[0m Lists all packages installed on the system    \n"
    "          \x1b[32m5) --search <package>\x1b[0m Searches for packages that match the name provided    \n"
    "          \x1b[32m6) --update\x1b[0m Checks if there are any updates to installed packages    \n"
    "          \x1b[32m7) --upgrade\x1b[0m Upgrades outdated packages    \n"
    "          \x1b[32m8) --package <path/to/package.ecmp>\x1b[0m Installs a package from file provided    \n"
    "          \x1b[32m9) --overwrite\x1b[0m Will overwrite installed packages    \n"
    "          \x1b[32m10) --debug <level 0-4>\x1b[0m Prints debug info    \n"
    "          \x1b[32m11) --verbose\x1b[0m Switches to verbose output    \x1b[0m \n";

int _install_source_(unsigned int* index);
int _remove_(unsigned int* index);
int _install_repo_(unsigned int* index);
int _create_binary_from_file(unsigned int* i);

int _set_debug_level_(unsigned int* i);
int _set_debug_unit(unsigned int* i);
int _set_verbose_(unsigned int* i);
int _set_overwrite_(unsigned int* i);

//test
int _list_(unsigned int* i);
int _update_(unsigned int* i);
int _upgrade_(unsigned int* i);
int _search_(unsigned int* i);


void* args[][2] = {
    //will test those later
    {"package",_install_source_},
    {"install",_install_repo_},
    {"list",_list_},
    {"update",_update_},
    {"upgrade",_upgrade_},
    {"remove",_remove_},
    {"search",_search_},
    {"debug",_set_debug_level_},
    //{"unit",_set_debug_unit},
    {"verbose", _set_verbose_},
    {"overwrite", _set_overwrite_}
    //{"create", _create_binary_from_file}
};

char** ARGV;

int main(int argc, char *argv[]) {

    

    dbg(2,"DEBUG Enabled!");

    if (argc == 1) {
       msg(ERROR, "No command specified add -h for information about how the command should be used.");
        return 1;
    }
    if (0 == strcmp(argv[1], "-h") || 0 == strcmp(argv[1], "--help")) {
        printf("%s", HELP);
        return 0;
    }

    if (0 == strcmp(argv[1], "-v") || 0 == strcmp(argv[1], "--version")){
	    printf(ART,C_FRONTEND_VERSION,version());
	    return 0;
    }
    // root check and --help and --version commands 
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

int _install_source_(unsigned int* i) {
    exit(install_package_source(ARGV[++(*i)],0));
}
int _remove_(unsigned int* i) {
    char* pkg = ARGV[++(*i)];
    msg(INFO, "removing %s", pkg); 
    exit(uninstall(pkg));
}
int _install_repo_(unsigned int* i) {
    struct package* pkg = calloc(1, sizeof(struct package));
    pkg->name = ARGV[++(*i)];

    char* format = get(pkg, pkg->name);

    if (format == NULL) {
        msg(ERROR, "Failed to download package %s", pkg->name);
        return 1;
    }

    f_install_package_source(pkg->name, 0, format);

    remove(pkg->name);
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

int _create_binary_from_file(unsigned int* i) {
    char* file = ARGV[++(*i)];
    char* binary = ARGV[++(*i)];

    create_binary_from_source(file,binary);

    return 0;
}
int _list_(unsigned int* i)
{
    list_installed();
    return 0;
}
int _update_(unsigned int* i)
{
    sync();
    update();
    return 0;
}
int _upgrade_(unsigned int* i)
{
    upgrade();
    return 0;
}
int _search_(unsigned int* i)
{
    char* input = ARGV[++(*i)];
    search(input);
    return 0;
}

