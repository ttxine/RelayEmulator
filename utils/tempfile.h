#pragma once
#include <string>

class TemporaryFile
{
  public:
    TemporaryFile();
    ~TemporaryFile();

    TemporaryFile(const TemporaryFile&) = delete;
    TemporaryFile& operator=(const TemporaryFile&) = delete;

  public:
    std::string GetPath() const { return path_; }

    int8_t Read() const;
    void Write(uint8_t to_write);

  private:
    int fd_;
    std::string path_;
};
