#pragma once

enum UseCase {
    INSTALL, CREATE, BINARY
};

int install_binary (std::string PName);
int move_binaries (std::vector<std::string> install_info);
void install_package(const std::string& PName, UseCase use);
void create_binary(std::string PName, std::string built_binaries, std::string dependencies);