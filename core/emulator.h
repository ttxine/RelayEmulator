#pragma once
#include <array>

#include "core/bus.h"

class Emulator
{
  public:
    Emulator(bool GUI_enabled = false);
    Emulator(const std::string& program_path,
             std::array<uint8_t, 2> input = {}, bool GUI_enabled = false);

    // Movable only
    Emulator(const Emulator&) = delete;
    Emulator& operator=(const Emulator&) = delete;
    Emulator(Emulator&&) = default;
    Emulator& operator=(Emulator&&) = default;

  public:
    // Executes the entire program.
    void Run();
    void Debug();

    // Performs one instruction.
    void Step();
    void Stop() noexcept;
    void Reset() noexcept;

    void Load(const std::string& program_path);
    void Input(uint8_t first, uint8_t second) noexcept;

    Bus::DebugInfo GetDebugInfo() const { return bus_.GetDebugInfo(); };
    void PrintDebugInfo() const;

    bool Stopped() const { return bus_.Stopped(); };

  private:
    // If GUI enabled, there is no need to print any information.
    bool gui_enabled_;

    Bus bus_;
};
