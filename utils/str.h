#pragma once
#include <string>

std::string strtolower(const std::string& str);
bool strisdigit(const std::string& str);

std::string to_hex_string(int val);

uint8_t asm_stoi(const std::string& Imm);
