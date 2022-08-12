#include <bitset>
#include <memory>
#include <array>
#include <iostream>

#include "core/emulator.h"

Emulator::Emulator(bool debug, bool GUI_enabled)
    : debug_(debug), GUI_enabled_(GUI_enabled)
{
}

Emulator::Emulator(const std::string& program_path, bool debug,
                   std::array<uint8_t, 2> input, bool GUI_enabled)
    : Emulator(debug, GUI_enabled)
{
  Load(program_path);
  Input(input[0], input[1]);

  UpdateBusDebugInfo();
}

void Emulator::Run()
{
  while (!main_bus_.Stopped())
  {
    Step();

    if (!GUI_enabled_ && debug_)
    {
      PrintBusDebugInfo();
      std::cin.get();
    }
  }

  if (!GUI_enabled_ && !debug_)
  {
    UpdateBusDebugInfo();
    PrintBusDebugInfo();
  }
}

void Emulator::Step()
{
  main_bus_.Clock();

  if (debug_ || main_bus_.Stopped())
  {
    UpdateBusDebugInfo();
  }
}

void Emulator::Reset()
{
  main_bus_.Reset();
  UpdateBusDebugInfo();
}

void Emulator::Load(const std::string& program_path)
{
  std::ifstream program(program_path, std::ios::in | std::ios::binary);

  if (program.fail())
  {
    throw std::runtime_error("can't open a file");
  }

  main_bus_.ConnectROM(std::unique_ptr<ROM>(new ROM(program)));
  UpdateBusDebugInfo();
}

void Emulator::Input(uint8_t first, uint8_t second)
{
  main_bus_.Input(first, second);
  UpdateBusDebugInfo();
}

Bus::DebugInfo Emulator::GetBusDebugInfo() const
{
  return debug_info_;
}

void Emulator::UpdateBusDebugInfo()
{
  debug_info_ = main_bus_.GetDebugInfo();
}

void Emulator::PrintBusDebugInfo() const
{
  printf("%s\n\n"
         "Registers:\n A: %s\n B: %s\n C: %s\n D: %s\n M: %s\n S: %s\n L: %s\n"
         "PC: %s\n\nFlags:\nCY: %d\n Z: %d\n S: %d\n\nInput:\n0x80: %s\n0x81:"
         " %s\n",
         debug_info_.instruction.c_str(),
         std::bitset<8>(debug_info_.r_A).to_string().c_str(),
         std::bitset<8>(debug_info_.r_B).to_string().c_str(),
         std::bitset<8>(debug_info_.r_C).to_string().c_str(),
         std::bitset<8>(debug_info_.r_D).to_string().c_str(),
         std::bitset<8>(debug_info_.r_M).to_string().c_str(),
         std::bitset<8>(debug_info_.r_S).to_string().c_str(),
         std::bitset<8>(debug_info_.r_L).to_string().c_str(),
         std::bitset<8>(debug_info_.r_PC).to_string().c_str(),
         debug_info_.f_CY,
         debug_info_.f_Z,
         debug_info_.f_S,
         std::bitset<8>(debug_info_.m_input_switches[0]).to_string().c_str(),
         std::bitset<8>(debug_info_.m_input_switches[1]).to_string().c_str());
}
