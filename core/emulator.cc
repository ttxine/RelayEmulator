#include <bitset>
#include <memory>
#include <array>
#include <iostream>

#include "core/emulator.h"

Emulator::Emulator(const std::string& program_path, bool debug,
                   std::array<uint8_t, 2> input) : debug_(debug)
{
  std::ifstream program(program_path, std::ios::in | std::ios::binary);

  if (program.fail())
  {
    throw std::runtime_error("Can't open a file");
  }

  memory_ = std::make_shared<Memory>(program, input);
  std::unique_ptr<CPU> cpu(new CPU(memory_));

  cpu_ = std::move(cpu);
}

void Emulator::Run()
{
  while (cpu_->IsRunning())
  {
    if (debug_)
    {
      printf("%.2x: %s\n\n", cpu_->GetRegister(CPU::kPC),
             std::bitset<16>(cpu_->Read(cpu_->GetRegister(CPU::kPC)))\
             .to_string().c_str());
    }

    Step();

    if (debug_)
    {
      PrintInfo();
      std::cin.get();
    }
  }

  PrintInfo();
}

void Emulator::Step()
{
  cpu_->Decode(cpu_->Fetch());
}

void Emulator::Input(uint8_t first, uint8_t second)
{
  memory_->Input(first, second);
}

void Emulator::PrintInfo() const
{
  printf("Registers:\n A: %s\n B: %s\n C: %s\n D: %s\n M: %s\n S: %s\n "
          "L: %s\nPC: %s\n\nFlags:\nCY: %d\n Z: %d\n S: %d\n\nInput:\n0x80: "
          "%s\n0x81: %s\n",
          std::bitset<8>(cpu_->GetRegister(CPU::kA)).to_string().c_str(),
          std::bitset<8>(cpu_->GetRegister(CPU::kB)).to_string().c_str(),
          std::bitset<8>(cpu_->GetRegister(CPU::kC)).to_string().c_str(),
          std::bitset<8>(cpu_->GetRegister(CPU::kD)).to_string().c_str(),
          std::bitset<8>(cpu_->GetRegister(CPU::kM)).to_string().c_str(),
          std::bitset<8>(cpu_->GetRegister(CPU::kS)).to_string().c_str(),
          std::bitset<8>(cpu_->GetRegister(CPU::kL)).to_string().c_str(),
          std::bitset<8>(cpu_->GetRegister(CPU::kPC)).to_string().c_str(),
          cpu_->GetFlag(CPU::Flag::kCY),
          cpu_->GetFlag(CPU::Flag::kZ),
          cpu_->GetFlag(CPU::Flag::kS),
          std::bitset<8>(cpu_->Read(0x80)).to_string().c_str(),
          std::bitset<8>(cpu_->Read(0x81)).to_string().c_str());
}

Emulator::State Emulator::GetCurrentState() const
{
  State state;

  state.A = cpu_->GetRegister(CPU::kA);
  state.B = cpu_->GetRegister(CPU::kB);
  state.C = cpu_->GetRegister(CPU::kC);
  state.D = cpu_->GetRegister(CPU::kD);
  state.M = cpu_->GetRegister(CPU::kM);
  state.S = cpu_->GetRegister(CPU::kS);
  state.L = cpu_->GetRegister(CPU::kL);
  state.PC = cpu_->GetRegister(CPU::kPC);

  for (int addr = 0; addr < state.program_data.size(); ++addr)
  {
    state.program_data[addr] = memory_->Read(addr);
  }

  for (int addr = 0; addr < state.input_switches.size(); ++addr)
  {
    int offset = state.program_data.size();
    state.input_switches[addr] = memory_->Read(addr + offset);
  }

  for (int addr = 0; addr < state.program_data.size(); ++addr)
  {
    int offset = state.program_data.size() + state.input_switches.size();
    state.unused[addr] = memory_->Read(addr + offset);
  }

  state.sign = cpu_->GetFlag(CPU::Flag::kS);
  state.zero = cpu_->GetFlag(CPU::Flag::kZ);
  state.carry = cpu_->GetFlag(CPU::Flag::kCY);

  return state;
}
