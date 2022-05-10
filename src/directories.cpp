#include <iostream>

void prepare_dirs(bool DEBUG)
{
    // Creating the directories if they don't exist
    mkdir(LOG_DIR.c_str(), 0777);
    mkdir(SPECIAL_DIR.c_str(), 0777);
    mkdir(PKG_DIR.c_str(), 0777);
    mkdir(SRC_DIR.c_str(), 0777);
    mkdir(BIN_DIR.c_str(), 0777);
    mkdir(BUILD_DIR.c_str(), 0777);
    mkdir(WORK_DIR.c_str(), 0777);
    mkdir(MAKE_DIR.c_str(), 0777);
    mkdir(DATA_DIR.c_str(), 0777);
    if (DEBUG) std::cout << "Directories created successfully, the problem is elsewhere."<< "\n";
}