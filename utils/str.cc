#include <sstream>
#include <iomanip>
#include <algorithm>
#include <stdexcept>

#include "utils/str.h"

std::string strtolower(const std::string& str)
{
  std::string lower = str;
  std::transform(lower.begin(), lower.end(), lower.begin(), tolower);

  return lower;
}

bool strisdigit(const std::string& str)
{
  for (char c : str)
  {
    if (!isdigit(c))
    {
      return false;
    }
  }

  return true;
}

std::string to_hex_string(int val, int width)
{
  std::stringstream hex;
  hex << std::setw(width) << std::setfill('0') << std::hex << val;

  return hex.str();
}

uint8_t asm_stoi(const std::string& Imm)
{
  if (!Imm.compare(0, 2, "0x") || !Imm.compare(0, 2, "0X"))
  {
    return std::stoi(std::string(Imm.begin() + 2, Imm.end()), nullptr, 16);
  }
  else if (!Imm.compare(0, 2, "0b") || !Imm.compare(0, 2, "0B"))
  {
    return std::stoi(std::string(Imm.begin() + 2, Imm.end()), nullptr, 2);
  }
  else if (!Imm.compare(0, 1, "0"))
  {
    return std::stoi(Imm, nullptr, 8);
  }
  else if (strisdigit(Imm))
  {
    return std::stoi(Imm, nullptr, 10);
  }
  throw std::runtime_error("invalid immediate value");
}
