#include <string>
#include <cstdint>
#include <algorithm>

#include "src/compiler.h"

uint16_t Compiler::EncodeInstruction(const std::string& instruction)
{
  TokenLine tline(instruction);

  std::string operation = tline.GetNextToken();
  std::transform(operation.begin(), operation.end(),
                 operation.begin(), ::tolower);

  if (operation == "halt")
    return 0x1000;
  else if (operation == "nop")
    return 0x0000;
  else if (operation == "load")
  {
    std::string G = tline.GetNextToken();
    std::string P = tline.GetNextToken();

    if (G.empty() || P.empty())
      throw CompilerException("Invalid instruction");

    if (::isdigit(P[0]))
      return 0x2000 | GetRegisterCode(G) << 8 | ImmStringToInt(P);
    else
      return 0x2800 | GetRegisterCode(G) << 8 | GetRegisterCode(P);
  }
  else if (operation == "store")
  {
    std::string G = tline.GetNextToken();
    std::string P = tline.GetNextToken();

    if (G.empty() || P.empty())
      throw CompilerException("Invalid instruction");

    if (::isdigit(P[0]))
      return 0x3000 | GetRegisterCode(G) << 8 | ImmStringToInt(P);
    else
      return 0x3800 | GetRegisterCode(G) << 8 | GetRegisterCode(P);   
  }
  else if (operation == "call")
  {
    std::string Cond = tline.GetNextToken();
    std::string Imm = tline.GetNextToken();

    if (Cond.empty())
      throw CompilerException("Invalid instruction");

    if (Imm.empty())
      return 0x8F00 | ImmStringToInt(Cond);

    return 0x8F00 | GetConditionCode(Cond) << 12 | ImmStringToInt(Imm);
  }
  else if (operation == "jmp")
  {
    std::string Cond = tline.GetNextToken();
    std::string Imm = tline.GetNextToken();

    if (Cond.empty())
      throw CompilerException("Invalid instruction");

    if (Imm.empty())
      return 0x8700 | ImmStringToInt(Cond);

    return 0x8700 | GetConditionCode(Cond) << 12 | ImmStringToInt(Imm);
  }
  else if (operation == "movi")
  {
    std::string Cond = tline.GetNextToken();
    std::string Gd = tline.GetNextToken();
    std::string Imm = tline.GetNextToken();

    if (Cond.empty() || Gd.empty())
      throw CompilerException("Invalid instruction");

    if (Imm.empty()) 
      return 0x8000 | GetConditionCode(Cond) << 12 | ImmStringToInt(Imm);
    
    return 0x8000 | GetConditionCode(Cond) << 12 | GetRegisterCode(Gd) << 8
           | ImmStringToInt(Imm);
  }
  else if (operation == "mov")
  {
    std::string Gd = tline.GetNextToken();
    std::string Gs = tline.GetNextToken();

    if (Gd.empty() || Gs.empty())
      throw CompilerException("Invalid instruction");

    return 0x1800 | GetRegisterCode(Gd) << 8 | GetRegisterCode(Gs) << 4;
  }
  else
    return EncodeALUInstruction(operation, tline);
}

uint16_t Compiler::EncodeALUInstruction(const std::string& operation,
                                        TokenLine& operands)
{
  std::string Gd = operands.GetNextToken();
  std::string Gs = operands.GetNextToken();
  std::string Op2 = operands.GetNextToken();

  if (Gd.empty() || Gs.empty())
    throw CompilerException("Invalid instruction");

  if (Op2.empty())
    return EncodeUnaryALUInstruction(operation, Gd, Gs);
  return EncodeBinaryALUInstruction(operation, Gd, Gs, Op2);
}

uint16_t Compiler::EncodeBinaryALUInstruction(const std::string& operation,
                                              const std::string& Gd,
                                              const std::string& Gs,
                                              const std::string& Op2)
{
  uint16_t icode = 0x4000;
  
  uint8_t opcode;
  if (operation == "adc") opcode = 0;
  else if (operation == "add") opcode = 1;
  else if (operation == "sbc") opcode = 2;
  else if (operation == "sub") opcode = 3;
  else if (operation == "and") opcode = 4;
  else if (operation == "or") opcode = 5;
  else if (operation == "xor") opcode = 6;
  else throw CompilerException("Invalid instruction");

  icode |= (opcode << 11);

  if (Gd != "F")
    icode |= (GetRegisterCode(Gd) << 8) | 0x0008;
  
  icode |= GetRegisterCode(Gs) << 4;
  
  try
  {
    icode |= GetRegisterCode(Op2);
  }
  catch (CompilerException& e)
  {
    icode |= (ImmStringToInt(Op2) & 0x07) | 0x0080;
  }

  return icode;
}

uint16_t Compiler::EncodeUnaryALUInstruction(const std::string& operation,
                                             const std::string& Gd,
                                             const std::string& Gs)
{
  uint16_t icode = 0x7800;

  uint8_t opcode;
  if (operation == "not") opcode = 0;
  else if (operation == "ror") opcode = 1;
  else if (operation == "shr") opcode = 2;
  else if (operation == "rcr") opcode = 3;
  else throw CompilerException("Invalid instruction");

  icode |= opcode << 1;
  
  if (Gd != "F")
    icode |= (GetRegisterCode(Gd)) << 8 | 0x08;

  icode |= GetRegisterCode(Gs) << 4;

  return icode;
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

inline uint8_t Compiler::ImmStringToInt(const std::string &Imm)
{
  return static_cast<uint8_t>(std::stoi(Imm, nullptr, 16));
}
