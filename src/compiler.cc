#include <string>
#include <cstdint>
#include <algorithm>

#include "src/compiler.h"

uint16_t Compiler::EncodeInstruction(const std::string& instruction)
{
  TokenLine tline(instruction);

  std::string operation = tline.GetNextToken();
  std::transform(operation.begin(), operation.begin(),
                 operation.begin(), ::tolower);
  if (operation == "halt")
    return 0x1000;
  else if (operation == "nop")
    return 0x0000;
  else if (operation == "load")
  {
    std::string G = tline.GetNextToken();
    std::string P = tline.GetNextToken();

    if (::isdigit(P[0]))
      return 0x2000 | GetRegisterCode(G) << 8
             | static_cast<uint8_t>(std::stoi(P, nullptr, 16));
    else
      return 0x2800 | GetRegisterCode(G) << 8 | GetRegisterCode(P);
  }
  else if (operation == "store")
  {
    std::string G = tline.GetNextToken();
    std::string P = tline.GetNextToken();

    if (::isdigit(P[0]))
      return 0x3000 | GetRegisterCode(G) << 8
             | static_cast<uint8_t>(std::stoi(P, nullptr, 16));
    else
      return 0x3800 | GetRegisterCode(G) << 8 | GetRegisterCode(P);   
  }
  else if (operation == "call")
  {
    std::string Cond = tline.GetNextToken();
    std::string Imm = tline.GetNextToken();

    if (Imm.empty())
    {
      Imm = Cond;
      return 0x8F00 | static_cast<uint8_t>(std::stoi(Imm, nullptr, 16));
    }

    return 0x8F00 | GetConditionCode(Cond) << 12
           | static_cast<uint8_t>(std::stoi(Imm, nullptr, 16));
  }

  throw CompilerException("Invalid instruction");
}

uint8_t Compiler::GetRegisterCode(const std::string& name)
{
  if (name == "A") return 0;
  else if (name == "B") return 1;
  else if (name == "C") return 2;
  else if (name == "D") return 3;
  else if (name == "M") return 4;
  else if (name == "S") return 5;
  else if (name == "L") return 6;
  else if (name == "PC") return 7;
  throw CompilerException("Invalid register");
}

uint8_t Compiler::GetConditionCode(const std::string& name)
{
  if (name == "A") return 0;
  else if (name == "Z") return 1;
  else if (name == "NS") return 2;
  else if (name == "C") return 3;
  else if (name == "NC") return 4;
  else if (name == "S") return 5;
  else if (name == "NZ") return 6;
  throw CompilerException("Invalid condition");
}
