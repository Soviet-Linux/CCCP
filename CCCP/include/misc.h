#pragma once

std::vector<std::string> split(std::string str, const std::string token);
const std::vector<std::string> open_spm(const std::string& PPath, const std::string& PKG_DIR);

template<typename ... Args>
std::string string_format( const std::string& format, Args ... args );