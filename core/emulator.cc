#include <bitset>
#include <memory>
#include <limits>
#include <iostream>
#include <arpa/inet.h>

#include "core/emulator.h"
#include "utils/str.h"

Emulator::Emulator(bool gui_enabled)
    : gui_enabled_(gui_enabled)
{
}

Emulator::Emulator(const std::string& program_path,
                   std::array<uint8_t, 2> input, bool gui_enabled)
    : Emulator(gui_enabled)
{
  Load(program_path);
  Input(input[0], input[1]);
}

void Emulator::Run()
{
  while (!bus_.Stopped())
  {
    Step();
  }

  if (!gui_enabled_)
  {
    PrintDebugInfo();
  }
}

void Emulator::Debug()
{
  while (!bus_.Stopped())
  {
    std::string command;
    while (command != "step" && command != "s")
    {
      std::cout << "(debug) ";
      std::getline(std::cin, command);

      if (command == "q" || command == "quit")
      {
        return;
      }
      else if (command == "h" || command == "help")
      {
        std::cout << "List of commands: \n"
                     "\n"
                     "  help, h                 Print this help message.\n"
                     "  step, s                 Execute next instruction.\n"
                     "  quit, q                 Exit debug." << std::endl;
      }
      else if (command != "s" && command != "step" && !command.empty())
      {
        std::cout << "Undefined command: \"" << command << "\". "
                      "Try \"help\"." << std::endl;
      }
    }

    Step();
    PrintDebugInfo();
  }
}

void Emulator::Step()
{
  if (!bus_.Stopped())
  {
    bus_.Cycle();
  }
  else
  {
    throw std::runtime_error("emulator: CPU is halted");
  }
}

void Emulator::Reset()
{
  bus_.Reset();
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

  bus_.ConnectROM(ROM(program_data));
}

void Emulator::Input(uint8_t first, uint8_t second)
{
  bus_.Input(first, second);
}

void Emulator::Stop()
{
  bus_.StopClock();
}

void Emulator::PrintDebugInfo() const
{
  Bus::DebugInfo info = GetDebugInfo();
  std::cout << "Instruction: " << info.instruction <<"\n"
               "\n"
               "Registers:\n"
               " A: " << std::bitset<8>(info.registers.A).to_string() << "     M: " << std::bitset<8>(info.registers.M).to_string() << "\n"
               " B: " << std::bitset<8>(info.registers.B).to_string() << "     S: " << std::bitset<8>(info.registers.S).to_string() << "\n"
               " C: " << std::bitset<8>(info.registers.C).to_string() << "     L: " << std::bitset<8>(info.registers.L).to_string() << "\n"
               " D: " << std::bitset<8>(info.registers.D).to_string() << "    PC: " << std::bitset<8>(info.registers.PC).to_string() << "\n\n"
               "Flags:\n"
               "CY: " << info.flags.CY << "    Z: " << info.flags.Z << "    S: " << info.flags.S << std::endl;
}
