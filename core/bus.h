#pragma once
#include <array>

#include "core/cpu.h"
#include "core/rom.h"

class Bus
{
  public:
    struct DebugInfo
    {
      bool is_running;

      uint16_t next_instruction;

      uint8_t r_A;
      uint8_t r_B;
      uint8_t r_C;
      uint8_t r_D;
      uint8_t r_M;
      uint8_t r_S;
      uint8_t r_L;
      uint8_t r_PC;

      std::array<uint16_t, ROM::kProgramDataSize> m_program_data;
      std::array<uint8_t, ROM::kInputSwitchesSize> m_input_switches;
      std::array<uint8_t, ROM::kUnusedSize> m_unused;

      bool f_S;
      bool f_Z;
      bool f_CY;
    };

  public:
    Bus(std::unique_ptr<ROM> rom);

    Bus(const Bus&) = delete;
    Bus& operator=(const Bus&) = delete;

  public:
    void Clock();

    uint16_t Read(uint8_t addr) const;
    void Write(uint8_t addr, uint8_t value);
    void Input(uint8_t first, uint8_t second);

    DebugInfo GetDebugInfo() const;

  private:
    std::unique_ptr<CPU> cpu_;
    std::unique_ptr<ROM> rom_;
};
