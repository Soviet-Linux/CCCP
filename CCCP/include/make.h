#pragma once

#include <sys/types.h>
#include "../include/nlohmann/json.hpp"

void make_pkg(const std::string& PName, const std::string& download_info, const std::string& build_info, const std::string& CURRENT_DIR);
int check_dependencies(const nlohmann::basic_json<>&, const std::string& DATA_DIR);
