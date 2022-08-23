#include <bitset>
#include <memory>
#include <iostream>
#include <arpa/inet.h>

#include "core/emulator.h"
#include "utils/str.h"

Emulator::Emulator(bool debug, bool gui_enabled)
    : debug_(debug), gui_enabled_(gui_enabled)
{
}

Emulator::Emulator(const std::string& program_path, bool debug,
                   std::array<uint8_t, 2> input, bool gui_enabled)
    : Emulator(debug, gui_enabled)
{
  Load(program_path);
  Input(input[0], input[1]);
}

void Emulator::Run()
{
  while (!main_bus_.Stopped())
  {
    Step();

    if (!gui_enabled_ && debug_)
    {
      PrintDebugInfo();
      std::cin.get();
    }
  }

  if (!gui_enabled_ && !debug_)
  {
    PrintDebugInfo();
  }
}

void Emulator::Step()
{
  if (!main_bus_.Stopped())
  {
    main_bus_.Clock();
  }
  else
  {
    throw std::runtime_error("emulator: CPU is halted or ROM isn't connected "
                             "to bus");
  }
}

void Emulator::Reset() noexcept
{
  main_bus_.Reset();
}

void Emulator::Load(const std::string& program_path)
{
  std::ifstream program(program_path, std::ios::in | std::ios::binary);

  if (program.fail())
  {
    throw std::runtime_error("emulator: can't open a file \"" + program_path +
                             "\"");
  }

  std::array<uint16_t, ROM::kProgramDataSize> program_data = {};

  uint16_t op;
  for (int addr = 0; program.read(reinterpret_cast<char*>(&op), sizeof(op)) &&
       addr < ROM::kProgramDataSize; ++addr)
  {
    program_data[addr] = ntohs(op);
  }

  main_bus_.ConnectROM(ROM(program_data));
}

void Emulator::Input(uint8_t first, uint8_t second) noexcept
{
  main_bus_.Input(first, second);
}

void Emulator::Stop() noexcept
{
  main_bus_.StopClock();
}

void Emulator::PrintDebugInfo() const
{
  Bus::DebugInfo info = GetDebugInfo();
  std::printf("Instruction: %s\n\n"
              "Registers:\n"
              " A: %s     M: %s\n"
              " B: %s     S: %s\n"
              " C: %s     L: %s\n"
              " D: %s    PC: %s\n\n"
              "Flags:\n"
              "CY: %d     Z: %d     S: %d\n",
              info.instruction.c_str(),
              std::bitset<8>(info.registers.A).to_string().c_str(),
              std::bitset<8>(info.registers.M).to_string().c_str(),
              std::bitset<8>(info.registers.B).to_string().c_str(),
              std::bitset<8>(info.registers.S).to_string().c_str(),
              std::bitset<8>(info.registers.C).to_string().c_str(),
              std::bitset<8>(info.registers.L).to_string().c_str(),
              std::bitset<8>(info.registers.D).to_string().c_str(),
              std::bitset<8>(info.registers.PC).to_string().c_str(),
              info.flags.CY,
              info.flags.Z,
              info.flags.S);
}
