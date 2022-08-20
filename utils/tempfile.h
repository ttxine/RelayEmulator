#pragma once
#include <string>

class TemporaryFile
{
  public:
    TemporaryFile();

    // Closes (if needed) and removes file.
    ~TemporaryFile();

    // Movable only
    TemporaryFile(const TemporaryFile&) = delete;
    TemporaryFile& operator=(const TemporaryFile&) = delete;
    TemporaryFile(TemporaryFile&&) = default;
    TemporaryFile& operator=(TemporaryFile&&) = default;

  public:
    std::string GetPath() const { return path_; }
    bool IsOpen() const { return is_open_; }

    int8_t Read();
    void Write(uint8_t to_write);

    // Close the file if it is no longer needed or before reopening.
    void Close();

  private:
    int fd_;
    std::string path_;
    bool is_open_;
};
