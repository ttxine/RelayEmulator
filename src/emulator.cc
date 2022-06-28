#include "emulator.h"

namespace relay
{
  Emulator::Emulator(bool debug)
  {
    memory_ = new Memory();
    cpu_ = new CPU(memory_);
    debug_ = debug;
  }

  void Emulator::Run()
  {
    while (cpu_->IsRunning())
    {
      Step();
      ++cycles_;

      if (debug_)
      {
        printf(
          "Registers:\n"
          " A: %s\n B: %s\n C: %s\n D: %s\n M: %s\n S: %s\n L: %s\nPC: %s\n"
          "Flags:\n"
          "CY: %d\n Z: %d\n S: %d\n",
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
          cpu_->GetFlag(CPU::Flag::kS)
        );
        std::cout << std::endl
              << "Input:" << std::endl
              << "0x80: " << std::bitset<8>(cpu_->Read(0x80)) << std::endl
              << "0x81: " << std::bitset<8>(cpu_->Read(0x81)) << std::endl;

        std::cin.get();
        std::system("clear");
      }
    }
    std::cout << std::endl << "On a real computer execution of program will "
              "take " << cycles_ * kSecPerCycle << " seconds to complete."
              << std::endl;
  }

  void Emulator::Step()
  {
    cpu_->Decode(cpu_->Fetch());
  }
}
