#pragma once
#include <string>

std::string strtolower(const std::string& str);
bool strisdigit(const std::string& str);

std::string create_temporary_file();
void unlink_temporary_file(const std::string& path);
