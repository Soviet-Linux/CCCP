#pragma once

// I don't know why I had to include this but if i dont it doesnt work properly
#include "../include/make.h"
//Prototypes
int add_data_to_db(const std::string& PName, pkg_data data);
pkg_data get_data_from_db(const std::string& PName);