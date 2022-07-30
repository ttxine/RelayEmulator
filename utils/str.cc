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

uint8_t asm_stoi(const std::string& Imm)
{
  if (!Imm.compare(0, 2, "0x"))
  {
    return std::stoi(Imm, nullptr, 16);
  }
  else if (strisdigit(Imm))
  {
    return std::stoi(Imm, nullptr, 10);
  }
  throw std::runtime_error("invalid immediate value");
}
