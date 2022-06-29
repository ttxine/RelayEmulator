#pragma once
#include <fstream>
#include <cstdint>

namespace relay
{
  class Memory {
    public:
      uint16_t Read(uint8_t);
      void Write(uint8_t addr, uint8_t value);

    private:
      uint16_t program_data_[64] = {
        0b0010000010000000,
        0b0010000110000001,
        0b0110100000000000,
        0b1001011100001011,
        0b0110100000010001,
        0b1001011100001011,
        0b0101100000001001,
        0b1100011100000010,
        0b0100100000001001,
        0b0101100100011000,
        0b1000011100000010,
        0b0100100000001001,
        0b0001000000000000
      };
      uint8_t input_switches_[16] = { 169, 52 };
      uint8_t unused_[112] = { 0x00 };
  };
}
