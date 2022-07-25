#pragma once
#include <string>

std::string strtolower(const std::string& str);
bool strisdigit(const std::string& str);

uint8_t asm_stoi(const std::string& Imm);

std::string create_temporary_file();
void unlink_temporary_file(const std::string& path);
