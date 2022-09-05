#include <cassert>
#include <stdexcept>

#include "core/bus.h"
#include "core/disassembler.h"

Bus::Bus()
{
  cpu_ = std::unique_ptr<CPU>(new CPU(this));
}

void Bus::Cycle()
{
  if (!Stopped())
  {
    cpu_->Cycle();
  }
}

uint16_t Bus::Read(uint8_t addr) const noexcept
{
  if (addr < ROM::kProgramDataSize)
  {
    return rom_.ReadProgramData(addr);
  }
  else if (addr < ROM::kProgramDataSize + ROM::kInputSwitchesSize)
  {
    return rom_.ReadInputSwitches(addr);
  }
  else
  {
    // Unused (read as 0)
    return rom_.ReadUnused(addr);
  }
}

void Bus::Write(uint8_t addr, uint8_t value) noexcept
{
}

void Bus::Input(uint8_t first, uint8_t second) noexcept
{
  rom_.Input(first, second);
}

void Bus::Reset() noexcept
{
  stopped_ = false;
  cpu_->Reset();
}

Bus::DebugInfo Bus::GetDebugInfo() const
{
  DebugInfo info;

  info.instruction = disassemble(cpu_->GetInstructionRegister());

  info.registers.A = cpu_->GetRegister(CPU::kA);
  info.registers.B = cpu_->GetRegister(CPU::kB);
  info.registers.C = cpu_->GetRegister(CPU::kC);
  info.registers.D = cpu_->GetRegister(CPU::kD);
  info.registers.M = cpu_->GetRegister(CPU::kM);
  info.registers.S = cpu_->GetRegister(CPU::kS);
  info.registers.L = cpu_->GetRegister(CPU::kL);
  info.registers.PC = cpu_->GetRegister(CPU::kPC);

  info.flags.S = cpu_->GetFlag(CPU::Flag::kS);
  info.flags.Z = cpu_->GetFlag(CPU::Flag::kZ);
  info.flags.CY = cpu_->GetFlag(CPU::Flag::kCY);

  for (int addr = 0; addr < info.memory.program_data.size(); ++addr)
  {
    info.memory.program_data[addr] = Read(addr);
  }

  uint8_t input_first = Read(0x80);
  uint8_t input_second = Read(0x81);
  for (int i = 0; i < ROM::kInputSwitchesSize / 2; ++i)
  {
    info.memory.input_switches[i * 2] = ((input_first << i) >> 7) & 0x01;
    info.memory.input_switches[i * 2 + 1] = ((input_second << i) >> 7) & 0x01;
  }

  return info;
};
