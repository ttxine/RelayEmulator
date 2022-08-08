#pragma once
#include <array>

#include "core/cpu.h"

class Emulator {
  public:
    struct State
    {
      uint8_t A;
      uint8_t B;
      uint8_t C;
      uint8_t D;
      uint8_t M;
      uint8_t S;
      uint8_t L;
      uint8_t PC;

      std::array<uint16_t, Memory::kProgramDataSize> program_data;
      std::array<uint8_t, Memory::kInputSwitchesSize> input_switches;
      std::array<uint8_t, Memory::kUnusedSize> unused;

      bool sign;
      bool zero;
      bool carry;
    };

  public:
    Emulator(bool debug = false);
    Emulator(const std::string& program_path, bool debug = false,
             std::array<uint8_t, 2> input = {});

    Emulator(const Emulator&) = delete;
    Emulator& operator=(const Emulator&) = delete;

  public:
    void Run();
    void Step();
    void Input(uint8_t first, uint8_t second);

    void PrintInfo() const;
    State GetCurrentState() const;

  private:
    bool debug_;

  private:
    std::unique_ptr<CPU> cpu_;
    std::shared_ptr<Memory> memory_;
};
