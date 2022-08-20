#pragma once
#include <array>

#include "core/bus.h"

class Emulator
{
  public:
    enum class State
    {
      kUnloaded,
      kReady,
      kRunning,
      kStopped
    };

  public:
    // A constructor that does not load the program. You need to connect the
    // ROM to the Bus manually via Load().
    Emulator(bool debug = false, bool GUI_enabled = false);
    Emulator(const std::string& program_path, bool debug = false,
             std::array<uint8_t, 2> input = {}, bool GUI_enabled = false);

    // Not copyable
    Emulator(const Emulator&) = delete;
    Emulator& operator=(const Emulator&) = delete;

  public:
    // Executes the entire program.
    void Run();

    // Performs one instruction.
    void Step();
    void Stop() noexcept { state_ = State::kStopped; }
    void Reset() noexcept;

    State GetCurrentState() const { return state_; }

    void Load(const std::string& program_path);
    void Input(uint8_t first, uint8_t second) noexcept;

    Bus::DebugInfo GetBusDebugInfo() const;
    void PrintBusDebugInfo() const;
    void UpdateBusDebugInfo() noexcept;

  private:
    bool debug_;

    // If GUI enabled, there is no need to print any information.
    bool GUI_enabled_;

  private:
    State state_ = State::kUnloaded;
    Bus main_bus_;
    Bus::DebugInfo debug_info_;
};
