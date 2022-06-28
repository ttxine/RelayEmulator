#pragma once
#include <cstdint>

namespace relay
{
  class Memory {
    public:
      uint16_t Read(uint8_t);
      void Write(uint8_t addr, uint8_t value);

    private:
      uint16_t program_data_[64] = {
        0b1000000100000001,
        0b0100101000011000,
        0b0001100000010000,
        0b0001100100100000,
        0b1000011100000001
      };
      uint8_t input_switches_[16] = { 0x00 };
      uint8_t unused_[112] = { 0x00 };
  };
}
