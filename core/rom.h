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
    ROM(const std::array<uint16_t, kProgramDataSize>& program = {},
        const std::array<uint8_t, kInputSwitchesSize / 8>& input = {});

  public:
    uint16_t ReadProgramData(uint8_t addr) const;
    uint8_t ReadInputSwitches(uint8_t addr) const;
    uint8_t ReadUnused(uint8_t addr) const;
    void Input(uint8_t first, uint8_t second);

  private:
    std::array<uint16_t, kProgramDataSize> program_data_;
    std::array<uint8_t, kInputSwitchesSize / 8> input_switches_;
};
