#pragma once
#include <array>

#include "core/bus.h"

class Emulator
{
  public:
    Emulator(bool debug = false, bool GUI_enabled = false);
    Emulator(const std::string& program_path, bool debug = false,
             std::array<uint8_t, 2> input = {}, bool GUI_enabled = false);

    // Movable only
    Emulator(const Emulator&) = delete;
    Emulator& operator=(const Emulator&) = delete;
    Emulator(Emulator&&) = default;
    Emulator& operator=(Emulator&&) = default;

  public:
    // Executes the entire program.
    void Run();

    // Performs one instruction.
    void Step();
    void Stop() noexcept;
    void Reset() noexcept;

    void Load(const std::string& program_path);
    void Input(uint8_t first, uint8_t second) noexcept;

    Bus::DebugInfo GetDebugInfo() const { return main_bus_.GetDebugInfo(); };
    void PrintDebugInfo() const;

    bool Stopped() const { return main_bus_.Stopped(); };

  private:
    bool debug_;

    // If GUI enabled, there is no need to print any information.
    bool gui_enabled_;

  private:
    Bus main_bus_;
};
