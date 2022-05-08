#pragma once



void rm_pkg (const std::string& PName,const std::string& DATA_DIR,const std::string& DATA_FILE,bool DEBUG);
bool check_pkg (const std::string& PPath,const std::string& DATA_DIR,bool DEBUG);
void make_pkg (const pkg_data& pkg,const std::string& MAKE_DIR,const std::string& BUILD_DIR,const std::string& LOG_DIR);