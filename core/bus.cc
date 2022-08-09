#include "core/bus.h"
#include <iostream>

Bus::Bus(std::unique_ptr<ROM> rom) : rom_(std::move(rom))
{
  cpu_ = std::unique_ptr<CPU>(new CPU(this));
};

void Bus::Clock()
{
  cpu_->Decode(cpu_->Fetch());
}

uint16_t Bus::Read(uint8_t addr) const
{
  return rom_->Read(addr);
}

void Bus::Write(uint8_t addr, uint8_t value)
{
  rom_->Write(addr, value);
}

void Bus::Input(uint8_t first, uint8_t second)
{
  rom_->Input(first, second);
}

Bus::DebugInfo Bus::GetDebugInfo() const
{
  DebugInfo info;

  info.is_running = !cpu_->Halted();

  info.next_instruction = cpu_->Fetch();

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
