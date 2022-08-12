#pragma once
#include <array>
#include <memory>
#include <cstdint>
#include <fstream>

class ROM
{
  public:
    static const std::size_t kProgramDataSize = 128;
    static const std::size_t kInputSwitchesSize = 16;
    static const std::size_t kUnusedSize = 112;

  public:
    ROM(std::ifstream& program, std::array<uint8_t, 2> input = {});

  public:
    uint16_t Read(uint8_t addr) const;
    void Write(uint8_t addr, uint8_t value);
    void Input(uint8_t first, uint8_t second);

  private:
    std::array<uint16_t, kProgramDataSize> program_data_ = {};
    std::array<uint8_t, kInputSwitchesSize> input_switches_ = {};
    std::array<uint8_t, kUnusedSize> unused_ = {};
};
