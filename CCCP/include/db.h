#pragma once

#include "../include/make.h"

int add_data_to_db(const std::string& PName, pkg_data data);
pkg_data get_data_from_db(const std::string& PName);