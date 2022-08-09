#pragma once
#include <array>

#include "core/bus.h"

class Emulator
{
  public:
    Emulator(const std::string& program_path, bool debug = false,
             std::array<uint8_t, 2> input = {}, bool GUI_enabled = false);

    Emulator(const Emulator&) = delete;
    Emulator& operator=(const Emulator&) = delete;

  public:
    void Run();
    void Step();

    Bus::DebugInfo GetBusDebugInfo() const;
    void PrintBusDebugInfo() const;
    void UpdateBusDebugInfo();

  private:
    bool debug_;
    bool GUI_enabled_;

  private:
    std::unique_ptr<Bus> main_bus_;
    Bus::DebugInfo debug_info_;
};
