#pragma once

#include "../include/spm.h"

void rm_pkg (const std::string& PPath,const std::string& DATA_DIR,bool DEBUG);
bool check_pkg (const std::string& PPath,const std::string& DATA_DIR,bool DEBUG);
void make_pkg (const pkg_data& pkg,const std::string& MAKE_DIR,const std::string& BUILD_DIR,const std::string& LOG_DIR,bool TESTING);