#include <arpa/inet.h>

#include "core/memory.h"

Memory::Memory(std::ifstream& program, std::array<uint8_t, 2> input)
{
  uint16_t opcode = 0x0000;
  uint8_t mem_used = 0;

  while (program.read(reinterpret_cast<char*>(&opcode), sizeof(opcode)) &&
         mem_used < kProgramDataSize)
  {
    program_data_[mem_used] = ntohs(opcode);
    ++mem_used;
  }

  Input(input[0], input[1]);
}

void Memory::Input(uint8_t first, uint8_t second)
{
  input_switches_[0] = first;
  input_switches_[1] = second;
}

uint16_t Memory::Read(uint8_t addr)
{
  if (addr < 0x80)
  {
    return program_data_[addr];
  }
  else if (addr < 0x90)
  {
    return input_switches_[addr - 0x80];
  }
  else
  {
    return unused_[addr - 0x90];
  }
}

void Memory::Write(uint8_t addr, uint8_t value)
{
}
