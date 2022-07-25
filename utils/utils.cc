#include <algorithm>
#include <stdexcept>
#include <unistd.h>

#include "utils/utils.h"

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

std::string create_temporary_file()
{
  std::string path = "/tmp/RelayEmulator~XXXXXX";

  int fd = mkstemp(&path[0]);
  close(fd);

  return path;
}

void unlink_temporary_file(const std::string& path)
{
  if (unlink(path.c_str()) == -1)
  {
    throw std::runtime_error("can't unlink a file: \"" + path + '"');
  }
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
