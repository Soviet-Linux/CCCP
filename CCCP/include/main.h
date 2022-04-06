#pragma once

enum UseCase {
    INSTALL, CREATE, BINARY
};

void install_package(const std::string& PName, UseCase use);
void create_binary(std::string PName, std::string built_binaries, std::string dependencies);