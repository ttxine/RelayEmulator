#include <stdexcept>
#include <unistd.h>

#include "utils/tempfile.h"

std::string create_temporary_file()
{
  std::string path = "/tmp/RelayEmulator~XXXXXX";

  int fd = mkstemp(&path[0]);

  if (fd == -1)
  {
    throw std::runtime_error("can't create temporary file");
  }

  close(fd);
  return path;
}

void unlink_temporary_file(const std::string& path)
{
  if (unlink(path.c_str()) == -1)
  {
    throw std::runtime_error("can't unlink temporary file: \"" + path + '\"');
  }
}
