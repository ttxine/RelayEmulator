#pragma once
#include <cstdint>
#include <fstream>

class Memory
{
  public:
    Memory(std::ifstream& program);
    Memory(std::ifstream& program, uint8_t* input);

  public:
    uint16_t Read(uint8_t);
    void Write(uint8_t addr, uint8_t value);

  private:
    uint16_t program_data_[64] = { 0x0000 };
    uint8_t input_switches_[16] = { 0x00 };
    uint8_t unused_[112] = { 0x00 };
};
