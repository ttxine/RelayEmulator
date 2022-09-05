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
      struct Registers
      {
        uint8_t A = 0x00;
        uint8_t B = 0x00;
        uint8_t C = 0x00;
        uint8_t D = 0x00;
        uint8_t M = 0x00;
        uint8_t S = 0x00;
        uint8_t L = 0x00;
        uint8_t PC = 0x00;
      } registers;

      struct Memory
      {
        std::array<uint16_t, ROM::kProgramDataSize> program_data = {};
        std::array<uint8_t, ROM::kInputSwitchesSize> input_switches = {};
      } memory;

      struct Flags
      {
        bool S = false;
        bool Z = false;
        bool CY = false;
      } flags;

      // Disassembled instruction
      std::string instruction = "NOP";
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
    void ConnectROM(const ROM& rom) { rom_ = rom; }

    // Performs one instruction cycle.
    void Cycle();

    // Stops the clock and program execution.
    void StopClock() { stopped_ = true; }
    bool Stopped() const { return stopped_; }

    uint16_t Read(uint8_t addr) const noexcept;
    void Write(uint8_t addr, uint8_t value) noexcept;

    // Emulates input switches. Used for easy input.
    void Input(uint8_t first, uint8_t second) noexcept;

    // Resets CPU and sets stopped_ to false.
    void Reset() noexcept;

    DebugInfo GetDebugInfo() const;

  private:
    bool stopped_ = false;

    std::unique_ptr<CPU> cpu_;
    ROM rom_;
};
