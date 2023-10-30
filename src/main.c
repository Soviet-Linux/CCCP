#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include </usr/include/spm/libspm.h>
#include </usr/include/spm/cutils.h>
#include </usr/include/spm/hashtable.h>
#include </usr/include/spm/globals.h>


const float C_FRONTEND_VERSION = 0.001;



char* ART = "\033[31;1;5m"
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
    "\x1b[32m Options: 1) -v Displays the version \n"
    "          2) -h Displays this message    \x1b[0m \n";

int _install_source_(unsigned int* index);
int _remove_(unsigned int* index);
int _install_repo_(unsigned int* index);
int _create_binary_from_file(unsigned int* i);

int _set_debug_level_(unsigned int* i);
int _set_debug_unit(unsigned int* i);
int _set_verbose_(unsigned int* i);
int _set_overwrite_(unsigned int* i);


void* args[][2] = {
    {"package",_install_source_},
    {"install",_install_repo_},
    {"remove",_remove_},
    {"debug",_set_debug_level_},
    {"unit",_set_debug_unit},
    {"verbose", _set_verbose_},
    {"overwrite", _set_overwrite_},
    {"create", _create_binary_from_file}
};

char** ARGV;

int main(int argc, char *argv[]) {

    

    dbg(2,"DEBUG Enabled!");

    if (argc == 1) {
       msg(ERROR, "No command specified add -h for information about how the command should be used.");
        return 1;
    }
    if (0 == strcmp(argv[1], "-h")) {
        printf("%s", HELP);
        return 0;
    }
    // root check and --help and --version commands 
    if (getuid() != 0 && (0 != strcmp(argv[1], "-h") || 0 != strcmp(argv[1], "-v"))) {
        msg(ERROR, "You need to be root to run this program for help type -h");
        return 1;
    } else if (0 == strcmp(argv[1], "-v")) {
        printf(ART,C_FRONTEND_VERSION,version());
        return 0;
    } else {
        dbg(2,"Running as root");
    }

    hashtable* hm = hm_init(args, sizeof(args)/sizeof(args[0]));

  

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
    exit(uninstall(ARGV[++(*i)]));
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
