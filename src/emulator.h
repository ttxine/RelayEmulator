#pragma once
#include "src/cpu.h"

namespace relay
{
  class Emulator {
    public:
      Emulator(const std::string& program_path, uint8_t* input,
               bool debug = false);
      ~Emulator();

    public:
      void Run();
      void Step();
      void PrintInfo();

    private:
      bool debug_ = false;

    private:
      CPU* cpu_ = nullptr;
  };
}
