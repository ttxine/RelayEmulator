#include "core/memory.h"

Memory::Memory(std::ifstream& program)
{
  uint8_t opcode[2];
  uint8_t mem_used = 0;

  while ((opcode[0] = program.get()) && mem_used < 64)
  {
    opcode[1] = program.get();
    if (opcode[1])
    {
      program_data_[mem_used] = (opcode[0] << 8) | opcode[1];
    }
    else
    {
      program_data_[mem_used] = opcode[0] << 8;
    }
    ++mem_used;
  }
}

Memory::Memory(std::ifstream& program, uint8_t* input)
:
Memory(program)
{
  input_switches_[0] = input[0];
  input_switches_[1] = input[1];
}

uint16_t Memory::Read(uint8_t addr)
{
  if (addr >= 0x00 && addr <= 0x7F)
  {
    return program_data_[addr];
  }
  else if (addr >= 0x80 && addr <= 0x8F)
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
  if (addr >= 0x00 && addr <= 0x7F)
  {
    return;
  }
  else if (addr >= 0x80 && addr <= 0x8F)
  {
    return;
  }
  else
  {
    return;
  }
}
