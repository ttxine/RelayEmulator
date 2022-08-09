#include <bitset>
#include <memory>
#include <array>
#include <iostream>

#include "core/emulator.h"

Emulator::Emulator(const std::string& program_path, bool debug,
                   std::array<uint8_t, 2> input, bool GUI_enabled)
    : debug_(debug), GUI_enabled_(GUI_enabled)
{
  std::ifstream program(program_path, std::ios::in | std::ios::binary);

  if (program.fail())
  {
    throw std::runtime_error("can't open a file");
  }

  std::unique_ptr<ROM> rom = std::unique_ptr<ROM>(new ROM(program, input));
  main_bus_ = std::unique_ptr<Bus>(new Bus(std::move(rom)));
  UpdateBusDebugInfo();
}

void Emulator::Run()
{
  while (debug_info_.is_running)
  {
    bool bNeedToPrint = !GUI_enabled_ && debug_;
  
    if (bNeedToPrint)
    {
      uint16_t next_instruction = debug_info_.next_instruction;
      printf("%.2x: %s\n\n", debug_info_.r_PC,
             std::bitset<16>(next_instruction).to_string().c_str());
    }

    Step();

    if (bNeedToPrint)
    {
      PrintBusDebugInfo();
      std::cin.get();
    }
  }

  if (!GUI_enabled_)
  {
    PrintBusDebugInfo();
  }
}

void Emulator::Step()
{
  main_bus_->Clock();
  UpdateBusDebugInfo();
}

Bus::DebugInfo Emulator::GetBusDebugInfo() const
{
  return debug_info_;
}

void Emulator::PrintBusDebugInfo() const
{
  printf("Registers:\n A: %s\n B: %s\n C: %s\n D: %s\n M: %s\n S: %s\n L: %s\n"
         "PC: %s\n\nFlags:\nCY: %d\n Z: %d\n S: %d\n\nInput:\n0x80: %s\n0x81:"
         " %s\n",
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

void Emulator::UpdateBusDebugInfo()
{
  debug_info_ = main_bus_->GetDebugInfo();
}
