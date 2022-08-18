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
    throw std::runtime_error("can't open temporary file \"" + path_ +
                             "\" by file descriptor.");
  }

  fd_ = fd;
  path_ = path;
}

TemporaryFile::~TemporaryFile()
{
  close(fd_);
  unlink(path_.c_str());
}

int8_t TemporaryFile::Read() const
{
  int8_t readed;

  int status = read(fd_, &readed, sizeof(readed));
  if (status == -1)
  {
    throw std::runtime_error("can't read from temporary file \"" + path_ + "\""
                             " by file descriptor: " +
                             std::string(strerror(errno)));
  }
  else if (status == 0)
  {
    return EOF;
  }

  return readed;
}

void TemporaryFile::Write(uint8_t to_write)
{
  int status = write(fd_, &to_write, sizeof(to_write));
  if (status == -1 || status == 0)
  {
    throw std::runtime_error("can't write to temporary file \"" + path_ + "\" "
                             "by file descriptor: " +
                             std::string(strerror(errno)));
  };
}
