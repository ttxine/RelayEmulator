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
    Emulator(bool debug = false, bool GUI_enabled = false);
    Emulator(const std::string& program_path, bool debug = false,
             std::array<uint8_t, 2> input = {}, bool GUI_enabled = false);

    Emulator(const Emulator&) = delete;
    Emulator& operator=(const Emulator&) = delete;

  public:
    void Run();
    void Step();
    void Stop() { state_ = State::kStopped; }
    void Reset();

    State GetCurrentState() const { return state_; }

    void Load(const std::string& program_path);
    void Input(uint8_t first, uint8_t second);

    Bus::DebugInfo GetBusDebugInfo() const;
    void PrintBusDebugInfo() const;
    void UpdateBusDebugInfo();

  private:
    bool debug_;
    bool GUI_enabled_;

  private:
    State state_ = State::kUnloaded;
    Bus main_bus_;
    Bus::DebugInfo debug_info_;
};
