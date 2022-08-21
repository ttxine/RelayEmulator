#pragma once
#include <array>

#include "core/cpu.h"
#include "core/rom.h"

class Bus
{
  public:
    // Used for GUI display and debugging.
    struct DebugInfo
    {
      // Disassembled instruction
      std::string instruction = "NOP";

      uint8_t r_A = 0x00;
      uint8_t r_B = 0x00;
      uint8_t r_C = 0x00;
      uint8_t r_D = 0x00;
      uint8_t r_M = 0x00;
      uint8_t r_S = 0x00;
      uint8_t r_L = 0x00;
      uint8_t r_PC = 0x00;

      std::array<uint16_t, ROM::kProgramDataSize> m_program_data = {};
      std::array<uint8_t, ROM::kInputSwitchesSize> m_input_switches = {};
      std::array<uint8_t, ROM::kUnusedSize> m_unused = {};

      bool f_S = false;
      bool f_Z = false;
      bool f_CY = false;
    };

  public:
    Bus();

    // Movable only
    Bus(const Bus&) = delete;
    Bus& operator=(const Bus&) = delete;
    Bus(Bus&&) = default;
    Bus& operator=(Bus&&) = default;

  public:
    // Used for easy program load.
    void ConnectROM(std::unique_ptr<ROM> rom);

    bool Stopped() const;

    // Performs the current CPU instruction if the clock isn't stopped.
    void Clock();

    // Stops the clock and program execution.
    void StopClock();

    uint16_t Read(uint8_t addr) const noexcept;
    void Write(uint8_t addr, uint8_t value) noexcept;

    // Emulates input switches. Used for easy input.
    void Input(uint8_t first, uint8_t second) noexcept;

    // Resets cpu_ and sets stopped_ to false.
    void Reset() noexcept;

    DebugInfo GetDebugInfo() const;

  private:
    bool stopped_ = false;

    std::unique_ptr<CPU> cpu_;
    std::unique_ptr<ROM> rom_;
};
