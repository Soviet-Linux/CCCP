#pragma once

//Prototypes
int install_binary(const std::string& PName);
void install_package (const std::string& PName);
void create_binary(const std::string& PName);
void prepare_dirs(); // create the directories if they dont exist