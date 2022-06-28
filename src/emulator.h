#pragma once
#include "cpu.h"
#include <bitset>

namespace relay
{
  class Emulator {
    public:
      const float kSecPerCycle = 0.66;

    public:
      Emulator(bool debug = false);

    public:
      void Run();
      void Step();

    private:
      bool debug_ = false;
      int cycles_ = 0;

    private:
      Memory* memory_ = nullptr;
      CPU* cpu_ = nullptr;
  };
}
