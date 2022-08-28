#include <stdexcept>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "utils/tempfile.h"

TemporaryFile::TemporaryFile()
{
  std::string path = "/tmp/RelayEmulator~XXXXXX";

  int fd = mkstemp(&path[0]);

  if (fd == -1)
  {
    throw std::runtime_error("can't create temporary file: " +
                             std::string(strerror(errno)));
  }

  path_ = path;
  fd_ = fd;
  is_open_ = true;
}

TemporaryFile::~TemporaryFile()
{
  if (is_open_) Close();
  unlink(path_.c_str());
}

void TemporaryFile::Write(uint8_t to_write)
{
  int status = write(fd_, &to_write, sizeof(to_write));
  if (status == -1 || status == 0)
  {
    throw std::runtime_error("can't write to temporary file \"" + path_ +
                             "\": " + std::string(strerror(errno)));
  };
}

void TemporaryFile::Close()
{
  close(fd_);
  is_open_ = false;
}
