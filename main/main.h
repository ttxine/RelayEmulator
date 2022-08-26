#pragma once
#include <array>
#include <string>

struct Options
{
  std::array<uint8_t, 2> input = {};
  bool debug = false;
  bool is_asm = false;
};

Options parse_options(int argc, char* argv[]);
void print_help(const std::string& binary);
