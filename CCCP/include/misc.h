#pragma once
#include "../include/nlohmann/json.hpp"

std::vector<std::string> split(std::string str, const std::string token);
nlohmann::basic_json<> open_spm(const std::string& PPath);

template<typename ... Args>
std::string string_format( const std::string& format, Args ... args );