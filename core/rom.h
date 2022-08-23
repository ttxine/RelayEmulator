#pragma once
#include <array>
#include <cstdint>
#include <fstream>

class ROM
{
  public:
    static const int kProgramDataSize = 128;
    static const int kInputSwitchesSize = 16;
    static const int kUnusedSize = 112;

  public:
    ROM(const std::array<uint16_t, 128>& program = {},
        const std::array<uint8_t, 2>& input = {});

  public:
    uint16_t ReadProgramData(uint8_t addr) const noexcept;
    uint8_t ReadInputSwitches(uint8_t addr) const noexcept;
    void Input(uint8_t first, uint8_t second) noexcept;

  private:
    std::array<uint16_t, kProgramDataSize> program_data_;
    std::array<uint8_t, kInputSwitchesSize / 8> input_switches_;
};
