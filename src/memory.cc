#include "memory.h"

namespace relay
{
  uint16_t Memory::ReadROM(uint8_t addr)
  {
    return program_data_[addr];
  }

  uint8_t Memory::ReadInput(uint8_t addr)
  {
    return input_switches_[addr - 0x80];
  }

  uint8_t Memory::ReadUnused(uint8_t addr)
  {
    return unused_[addr - 0x90];
  }

  void Memory::WriteUnused(uint8_t addr, uint8_t value)
  {
    input_switches_[addr - 0x90] = value;
    return;
  }
}
