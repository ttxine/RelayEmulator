#pragma once
#include "core/cpu.h"

class Emulator {
  public:
    Emulator(const std::string& program_path, uint8_t* input);
    Emulator(const std::string& program_path, uint8_t* input,
             bool debug);

    Emulator(const Emulator&) = delete;
    Emulator& operator=(const Emulator&) = delete;

  public:
    void Run();
    void Step();
    void PrintInfo();

  private:
    bool debug_ = false;

  private:
    std::unique_ptr<CPU> cpu_;
};
