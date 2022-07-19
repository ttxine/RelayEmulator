#include "src/compiler.h"

uint16_t Compiler::EncodeInstruction(const std::string& instruction)
{
  token::TokenLine tline(instruction);

  token::TokenLine::Token itoken = tline.GetNextToken();

  if (!token::is_intruction(itoken))
    throw CompilerException("Invalid instruction");

  std::transform(itoken.str.begin(), itoken.str.end(),
                 itoken.str.begin(), ::tolower);

  if (itoken.str == "halt")
    return 0x1000;
  else if (itoken.str == "nop")
    return 0x0000;
  else if (itoken.str == "load")
  {
    token::TokenLine::Token G = tline.GetNextToken();
    token::TokenLine::Token P = tline.GetNextToken();

    if (!token::is_operand(G) || !token::is_operand(G))
      throw CompilerException("Invalid operands");

    if (::isdigit(P.str[0]))
      return 0x2000 | GetRegisterCode(G.str) << 8 | ImmStringToInt(P.str);
    else
      return 0x2800 | GetRegisterCode(G.str) << 8 | GetRegisterCode(P.str);
  }
  else if (itoken.str == "store")
  {
    token::TokenLine::Token G = tline.GetNextToken();
    token::TokenLine::Token P = tline.GetNextToken();

    if (!token::is_operand(G) || !token::is_operand(G))
      throw CompilerException("Invalid operands");

    if (::isdigit(P.str[0]))
      return 0x3000 | GetRegisterCode(G.str) << 8 | ImmStringToInt(P.str);
    else
      return 0x3800 | GetRegisterCode(G.str) << 8 | GetRegisterCode(P.str);   
  }
  else if (itoken.str == "call")
  {
    token::TokenLine::Token cond = tline.GetNextToken();
    token::TokenLine::Token Imm = tline.GetNextToken();

    if (!token::is_operand(cond))
      throw CompilerException("Invalid operands");

    if (token::is_none(Imm))
      return 0x8F00 | ImmStringToInt(cond.str);

    if (!token::is_operand(Imm))
        throw CompilerException("Invalid operands");

    return 0x8F00 | GetConditionCode(cond.str) << 12 | ImmStringToInt(Imm.str);
  }
  else if (itoken.str == "jmp")
  {
    token::TokenLine::Token cond = tline.GetNextToken();
    token::TokenLine::Token Imm = tline.GetNextToken();

    if (!token::is_operand(cond))
      throw CompilerException("Invalid operands");

    if (token::is_none(Imm))
      return 0x8700 | ImmStringToInt(cond.str);

    if (!token::is_operand(Imm))
      throw CompilerException("Invalid operands");

    return 0x8700 | GetConditionCode(cond.str) << 12 | ImmStringToInt(Imm.str);
  }
  else if (itoken.str == "movi")
  {
    token::TokenLine::Token cond = tline.GetNextToken();
    token::TokenLine::Token Gd = tline.GetNextToken();
    token::TokenLine::Token Imm = tline.GetNextToken();

    if (!token::is_operand(cond) || !token::is_operand(Gd))
      throw CompilerException("Invalid instruction");

    if (token::is_none(Imm)) 
      return 0x8000 | GetRegisterCode(cond.str) << 12 | ImmStringToInt(Gd.str);

    if (!token::is_operand(Imm))
      throw CompilerException("Invalid operands");

    return 0x8000 | GetConditionCode(cond.str) << 12
           | GetRegisterCode(Gd.str) << 8 | ImmStringToInt(Imm.str);
  }
  else if (itoken.str == "mov")
  {
    token::TokenLine::Token Gd = tline.GetNextToken();
    token::TokenLine::Token Gs = tline.GetNextToken();

    if (!token::is_operand(Gd) || !token::is_operand(Gs))
      throw CompilerException("Invalid instruction");

    return 0x1800 | GetRegisterCode(Gd.str) << 8 | GetRegisterCode(Gs.str) << 4;
  }
  else
    return EncodeALUInstruction(itoken.str, tline);
}

uint16_t Compiler::EncodeALUInstruction(const std::string& operation,
                                        token::TokenLine& operands)
{
  token::TokenLine::Token Gd = operands.GetNextToken();
  token::TokenLine::Token Gs = operands.GetNextToken();
  token::TokenLine::Token Op2 = operands.GetNextToken();

  if (!token::is_operand(Gd) || !token::is_operand(Gs))
    throw CompilerException("Invalid instruction");

  if (token::is_none(Op2))
    return EncodeUnaryALUInstruction(operation, Gd.str, Gs.str);

  if (!token::is_operand(Op2))
      throw CompilerException("Invalid operands");

  return EncodeBinaryALUInstruction(operation, Gd.str, Gs.str, Op2.str);
}

uint16_t Compiler::EncodeBinaryALUInstruction(const std::string& operation,
                                              const std::string& Gd,
                                              const std::string& Gs,
                                              const std::string& Op2)
{
  uint16_t icode = 0x4000;

  if (operation == "adc");
  else if (operation == "add") icode |= 1 << 11;
  else if (operation == "sbc") icode |= 2 << 11;
  else if (operation == "sub") icode |= 3 << 11;
  else if (operation == "and") icode |= 4 << 11;
  else if (operation == "or") icode |= 5 << 11;
  else if (operation == "xor") icode |= 6 << 11;
  else throw CompilerException("Invalid instruction");

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

  if (operation == "not");
  else if (operation == "ror") icode |= 1 << 1;
  else if (operation == "shr") icode |= 2 << 1;
  else if (operation == "rcr") icode |= 3 << 1;
  else throw CompilerException("Invalid instruction");
  
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
