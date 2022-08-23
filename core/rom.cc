#include <arpa/inet.h>

#include "core/rom.h"

ROM::ROM(const std::array<uint16_t, 128>& program,
         const std::array<uint8_t, 2>& input)
    : program_data_(program), input_switches_(input)
{
}

void ROM::Input(uint8_t first, uint8_t second) noexcept
{
  input_switches_[0] = first;
  input_switches_[1] = second;
}

uint16_t ROM::ReadProgramData(uint8_t addr) const noexcept
{
  if (addr < kProgramDataSize)
  {
    return program_data_[addr];
  }
  else
  {
    return 0x0000;
  }
}

uint8_t ROM::ReadInputSwitches(uint8_t addr) const noexcept
{
  uint8_t data = 0x00;

  switch (addr)
  {
    case 0x80: data = input_switches_[0]; break;
    case 0x81: data = input_switches_[1]; break;
    default: break;
  }

  return data;
}
