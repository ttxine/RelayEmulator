#include "memory.h"

namespace relay
{
  uint16_t Memory::Read(uint8_t addr)
  {
    if (addr >= 0x00 && addr <= 0x7F)
      return program_data_[addr];
    else if (addr >= 0x80 && addr <= 0x8F)
      return input_switches_[addr - 0x80];
    else
      return unused_[addr - 0x90];
  }

  void Memory::Write(uint8_t addr, uint8_t value)
  {
    if (addr >= 0x00 && addr <= 0x7F)
      return;
    else if (addr >= 0x80 && addr <= 0x8F)
      return;
    else
      unused_[addr - 0x90] = value;
  }
}
