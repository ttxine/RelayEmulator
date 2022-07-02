#pragma once
#include <string>
#include <algorithm>

#include "cpu.h"

namespace relay
{
  class Compiler
  {
    public:
      Compiler(std::ifstream& program)
      {
        Compile(program);
      }

    private:
      uint16_t* Compile(std::ifstream& program)
      {
        uint16_t* program_data = new uint16_t[64] { 0x0000 };
        uint8_t rom_used = 0;

        char buf[256];

        while(program.getline(buf, 256))
        {
          std::string line(buf);

          std::transform(line.begin(), line.end(), line.begin(),
                  [](unsigned char c){ return std::tolower(c); });

          size_t pos = 0;
          for (char c : line)
            if (isalpha(c)) pos += 1;

          std::string opcode = line.substr(0, pos - 1);

          if (opcode.compare("halt"))
          {
            program_data[rom_used] = CPU::kHALT;
          }
          else if (opcode.compare("load"))
          {
            std::string operands;

            for (size_t i = pos; i < line.length(); ++i)
              if (!isspace(line[i]))
                operands.push_back(line[i]);

            std::string G;
            std::string P;

            for (pos = 0; operands[pos] != ',' && pos < operands.length();
                 ++pos)
              G.push_back(operands[pos]);

            if (G.length() == 0 || pos == operands.length())
              throw std::exception();
            else
              ++pos;

            while (isalpha(operands[pos]) && pos < operands.length())
            {
              P.push_back(operands[pos]);
              ++pos;
            }

            if (P.length() == 0)
              throw std::exception();

            uint8_t G_code = GetRegisterCodeByName(G);
            uint8_t P_code = GetRegisterCodeByName(P);
            program_data[rom_used] = CPU::kLOAD | G_code << 8 | P_code;
          }
          else
          {
            program_data[rom_used] = CPU::kNOP;
          }
          ++rom_used;
        }

        return program_data;
      }

      uint8_t GetRegisterCodeByName(std::string name)
      {
        if (name.compare("A"))        return CPU::kA;
        else if (name.compare("B"))   return CPU::kB;
        else if (name.compare("C"))   return CPU::kC;
        else if (name.compare("D"))   return CPU::kD;
        else if (name.compare("M"))   return CPU::kM;
        else if (name.compare("S"))   return CPU::kS;
        else if (name.compare("L"))   return CPU::kL;
        else if (name.compare("PC"))  return CPU::kPC;
        else
          throw std::exception();
      }
  };
}
