#pragma once

// I had to include this but I don't know why
#include <string>
#include <sys/types.h>


//Prototypes
void move_binaries(const std::string& BUILD_DIR ,const std::string& ROOT);
int check_dependencies (const std::vector<std::string>& dependencies, const std::string& DATA_DIR); 
int install_binary(const std::string& PName);
void install_package (const std::string& PName);
void create_binary(const std::string& PName);