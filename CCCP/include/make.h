#pragma once

#include <sys/types.h>

std::vector<std::string> init_pkg_list(std::string DATA_DIR);
void make_pkg(const std::string& PName, const std::string& download_info, const std::string& build_info, const std::string& CURRENT_DIR);
bool check_dependencies(std::string& dependencies, const std::string& DATA_DIR);
