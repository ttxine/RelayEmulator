#include <stdexcept>

#include "core/bus.h"
#include "core/disassembler.h"

Bus::Bus()
{
  cpu_ = std::unique_ptr<CPU>(new CPU(this));
}

void Bus::ConnectROM(std::unique_ptr<ROM> rom)
{
  if (!rom)
  {
    throw std::runtime_error("can't connect ROM to Bus: invalid ROM");
  }

  rom_ = std::move(rom);
}

void Bus::Clock()
{
  if (!Stopped()) cpu_->Clock();
}

void Bus::StopClock()
{
  stopped_ = true;
}

bool Bus::Stopped() const
{
  return stopped_;
}

uint16_t Bus::Read(uint8_t addr) const noexcept
{
  return rom_->Read(addr);
}

void Bus::Write(uint8_t addr, uint8_t value) noexcept
{
}

void Bus::Input(uint8_t first, uint8_t second) noexcept
{
  rom_->Input(first, second);
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

  info.r_A = cpu_->GetRegister(CPU::kA);
  info.r_B = cpu_->GetRegister(CPU::kB);
  info.r_C = cpu_->GetRegister(CPU::kC);
  info.r_D = cpu_->GetRegister(CPU::kD);
  info.r_M = cpu_->GetRegister(CPU::kM);
  info.r_S = cpu_->GetRegister(CPU::kS);
  info.r_L = cpu_->GetRegister(CPU::kL);
  info.r_PC = cpu_->GetRegister(CPU::kPC);

  for (int addr = 0; addr < info.m_program_data.size(); ++addr)
  {
    info.m_program_data[addr] = Read(addr);
  }

  for (int addr = 0; addr < info.m_input_switches.size(); ++addr)
  {
    int offset = info.m_program_data.size();
    info.m_input_switches[addr] = Read(addr + offset);
  }

  for (int addr = 0; addr < info.m_program_data.size(); ++addr)
  {
    int offset = info.m_program_data.size() + info.m_input_switches.size();
    info.m_unused[addr] = Read(addr + offset);
  }

  info.f_S = cpu_->GetFlag(CPU::Flag::kS);
  info.f_Z = cpu_->GetFlag(CPU::Flag::kZ);
  info.f_CY = cpu_->GetFlag(CPU::Flag::kCY);

  return info;
};
