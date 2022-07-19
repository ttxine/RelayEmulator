#pragma once
#include <iostream>
#include <bitset>

#include "src/cpu.h"

namespace relay
{
  class Emulator {
    public:
      const float kSecPerCycle = 0.66;

    public:
      Emulator(const std::string& program_path, bool debug = false);
      Emulator(const std::string& program_path, uint8_t* input,
               bool debug = false);

    public:
      void Run();
      void Step();
      void PrintInfo();

    private:
      bool debug_ = false;
      int cycles_ = 0;

    private:
      Memory* memory_ = nullptr;
      CPU* cpu_ = nullptr;
  };
}
