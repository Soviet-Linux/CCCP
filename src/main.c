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
    "          \x1b[32m       --clean\x1b[0m                             Cleans up the cache directory    \n"
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

int _clean_up_(unsigned int* i);

//test
int _update_(unsigned int* i);
int _upgrade_(unsigned int* i);
int _search_(unsigned int* i);
int _set_yes_(unsigned int* i);
int _set_no_(unsigned int* i);

void handle_inputs(struct package* pkg);
void ask_to_preview_pkg(char* name);


void* args[][2] = {
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
    {"clean", _clean_up_},
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
int _install_source_(unsigned int* i) 
{
    struct package* pkg = calloc(1, sizeof(struct package));
    char* name = ARGV[++(*i)];

    ask_to_preview_pkg(name);

    // TODO:
    // Accept a --opt "opt" argument then 
    // Check if a dependency is in the opt string

    // Attempt to open the package archive
    if (open_pkg(name, pkg, getenv("SOVIET_DEFAULT_FORMAT")) != 0) {
        msg(ERROR, "Failed to open package");
        return -1;
    }

    dbg(1, "Checking optional dependencies...");


    // Checking optional dependencies
    if (pkg->optionalCount > 0) {
        dbg(1, "Checking optional dependencies...");
        check_optional_dependencies(pkg->optional, pkg->optionalCount);
    }

    // TODO:
    // Accept a --in "in or --in def argument 
    // Then check if the number of arguments in
    // The string is equal to number of inputs
    // If so, supply the inputs NQA
    dbg(1, "Handling inputs...");

    handle_inputs(pkg);
    
    dbg(1, "Installing %s...", pkg->name);

    f_install_package_source(name, 0, "local");

    return 0;
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

    char* pkg_name = calloc(strlen(pkg->name) + strlen(getenv("SOVIET_DEFAULT_FORMAT")) + 2, sizeof(char));
    if(!strstr(pkg->name, ".ecmp"))
    {
        sprintf(pkg_name, "%s.%s", pkg->name, getenv("SOVIET_DEFAULT_FORMAT"));
    }
        else
        {
            pkg_name = strdup(pkg->name);
        }

    int num_results;
    char** results = search(pkg_name, &num_results);

    char* repo;

    if(results != NULL && num_results > 0)
    {
        for ( int i = 0; i < num_results; i++)
        {
            // Package name
            char* temp_1 = strtok(results[i], ">");
            // Repo it's in
            char* temp_2 = strchr(results[i], '\0') + 1;

            if(strcmp(getenv("SOVIET_DEFAULT_REPO"), temp_2) == 0)
            {
                repo = temp_2;
                break;
            }
                else if (i == num_results) 
                {
                    repo = temp_2;
                }
        }
    
        if (repo == NULL) {
            msg(ERROR, "Failed to download package %s", pkg->name);
            return 1;
        }

        get(pkg, repo, pkg->name);

        ask_to_preview_pkg(pkg->name);

        // TODO:
        // Accept a --opt "opt" argument then 
        // Check if a dependency is in the opt string

        // Attempt to open the package archive
        if (open_pkg(pkg->name, pkg, getenv("SOVIET_DEFAULT_FORMAT")) != 0) {
            msg(ERROR, "Failed to open package");
            return -1;
        }

        dbg(1, "Checking optional dependencies...");


        // Checking optional dependencies
        if (pkg->optionalCount > 0) {
            dbg(1, "Checking optional dependencies...");
            check_optional_dependencies(pkg->optional, pkg->optionalCount);
        }

        // TODO:
        // Accept a --in "in or --in def argument 
        // Then check if the number of arguments in
        // The string is equal to number of inputs
        // If so, supply the inputs NQA
        dbg(1, "Handling inputs...");

        handle_inputs(pkg);
        
        dbg(1, "Installing %s...", pkg->name);

        f_install_package_source(pkg->name, 0, repo);

        remove(pkg->name);

        return 0;
    }
    else 
        {
            msg(ERROR, "No such package %s", pkg_name);
            return -1;
        }
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
    int num_results;
    char** results = search(input, &num_results);

    if(results != NULL)
    {
        for ( int i = 0; i < num_results; i++)
        {
            // Package name
            char* temp_1 = strtok(results[i], ">");
            // Repo it's in
            char* temp_2 = strchr(results[i], '\0') + 1;
            printf("Found %s in %s \n", temp_1, temp_2);
        }
    }
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

void ask_to_preview_pkg(char* name)
{
    char* str = calloc(2, sizeof(char));

    msg(INFO, "Do you want to view the source for %s before installing? y/N", name);
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
        sprintf(cmd, "cat %s", name);
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
            remove(name);

            free(str);
            free(str_2);

            msg(FATAL, "Aborting...");
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
    free(str);

}

void handle_inputs(struct package* pkg)
{
    if(pkg->inputsCount > 0)
    {
        for (int i = 0; i < pkg->inputsCount; i++) 
            {
                msg(INFO, "%s", pkg->inputs[i]);
                char* str = calloc(MAX_PATH, sizeof(char));

                if(!OVERWRITE_CHOISE)
                {
                    char* res = fgets(str, MAX_PATH-1, stdin);
                    dbg(1, "Checking if enter was pressed");
                    if ( strchr(str, '\n') == NULL )
                    {
                        while ((getchar()) != '\n');
                    }

                    int k = 0;

                    while (str[k] != '\n' && str[k] != '\0')
                    {
                        dbg(1, "Checking if input %c is bad", str[k]);

                        if(str[k] == '~' 
                        | str[k] == '`' 
                        | str[k] == '#' 
                        | str[k] == '$' 
                        | str[k] == '&' 
                        | str[k] == '*' 
                        | str[k] == '(' 
                        | str[k] == ')' 
                        | str[k] == '\\'  
                        | str[k] == '|' 
                        | str[k] == '[' 
                        | str[k] == ']' 
                        | str[k] == '{'
                        | str[k] == '}' 
                        | str[k] == '\'' 
                        | str[k] == ';' 
                        | str[k] == '\\' 
                        | str[k] == '<' 
                        | str[k] == '>' 
                        | str[k] == '?' 
                        | str[k] == '!')
                        {
                            str[k] = ' ';
                        }
                        k++;
                    }

                    dbg(1, "replacing last new line");


                    if (str[k] == '\n')
                    {
                        str[k] = '\0';
                    }

                    char* in = calloc(128, sizeof(char));
                    sprintf(in, "INPUT_%d", i);
                    setenv(in, str, 0);
                    free(in);
                }
                    else
                    {
                        sprintf(str, "%s", USER_CHOISE[0]);
                        char* in = calloc(128, sizeof(char));
                        sprintf(in, "INPUT_%d", i);
                        setenv(in, str, 0);
                        free(in);
                    }
                    free(str);
            }
    }    
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
}