#include <bitset>
#include <iostream>

#include "src/emulator.h"

namespace relay
{
  Emulator::Emulator(const std::string& program_path, uint8_t* input,
                     bool debug)
  {
    std::ifstream program(program_path, std::ios::in | std::ios::binary);

    if (program.fail())
    {
      throw std::runtime_error("Can't open a file");
    }

    Memory* memory = new Memory(program, input);
    cpu_ = new CPU(memory);
    debug_ = debug;
  }

  Emulator::~Emulator()
  {
    delete cpu_;
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

  void Emulator::PrintInfo()
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
}
