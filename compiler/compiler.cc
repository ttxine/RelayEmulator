#include <fstream>

#include "utils/utils.h"
#include "compiler/compiler.h"
#include "compiler/preprocessor.h"

std::string Compiler::Run()
{
  Preprocessor prep(to_compile_);
  to_compile_ = prep.Run();

  std::ifstream in(to_compile_);
  if (in.fail())
  {
    throw std::runtime_error("can't open a file: \"" + to_compile_ + '"');
  }

  std::string path = create_temporary_file();
  std::ofstream out(path);

  std::string line;
  while (std::getline(in, line))
  {
    uint16_t icode = EncodeInstruction(line);
    out << static_cast<uint8_t>(icode >> 8) << static_cast<uint8_t>(icode);

    // std::cout << line << std::hex << icode << std::endl;
  }

  unlink_temporary_file(to_compile_);

  return path;
}

uint16_t Compiler::EncodeInstruction(const std::string& instruction)
{
  TokenLine tline(strtolower(instruction));

  TokenLine::Token itoken = tline.GetNextToken();

  if (!is_instruction(itoken))
  {
    throw InvalidInstructionException();
  }

  if (itoken.str == "halt")
  {
    return EncodeHaltInstruction();
  }
  else if (itoken.str == "nop")
  {
    return EncodeNopInstruction();
  }
  else if (itoken.str == "load")
  {
    TokenLine::Token G = tline.GetNextToken();
    TokenLine::Token P = tline.GetNextToken();

    return EncodeLoadInstruction(G, P);
  }
  else if (itoken.str == "store")
  {
    TokenLine::Token G = tline.GetNextToken();
    TokenLine::Token P = tline.GetNextToken();

    return EncodeStoreInstruction(G, P);
  }
  else if (itoken.str == "call")
  {
    TokenLine::Token cond = tline.GetNextToken();
    TokenLine::Token Imm = tline.GetNextToken();

    return EncodeCallInstruction(cond, Imm);
  }
  else if (itoken.str == "jmp")
  {
    TokenLine::Token cond = tline.GetNextToken();
    TokenLine::Token Imm = tline.GetNextToken();

    return EncodeJmpInstruction(cond, Imm);
  }
  else if (itoken.str == "movi")
  {
    TokenLine::Token cond = tline.GetNextToken();
    TokenLine::Token Gd = tline.GetNextToken();
    TokenLine::Token Imm = tline.GetNextToken();

    return EncodeMoviInstruction(cond, Gd, Imm);
  }
  else if (itoken.str == "mov")
  {
    TokenLine::Token Gd = tline.GetNextToken();
    TokenLine::Token Gs = tline.GetNextToken();

    return EncodeMovInstruction(Gd, Gs);
  }
  else
  {
    TokenLine::Token Gd = tline.GetNextToken();
    TokenLine::Token Gs = tline.GetNextToken();
    TokenLine::Token Op2 = tline.GetNextToken();

    return EncodeAluInstruction(itoken, Gd, Gs, Op2);
  }
}

uint16_t Compiler::EncodeHaltInstruction()
{
  return 0x1000;
}

uint16_t Compiler::EncodeNopInstruction()
{
  return 0x0000;
}

uint16_t Compiler::EncodeLoadInstruction(TokenLine::Token& G,
                                         TokenLine::Token& P)
{
  if (!is_operand(G) || !is_operand(G))
  {
    throw InvalidOperandException();
  }

  if (::isdigit(P.str[0]))
  {
    return 0x2000 | GetRegisterCode(G.str) << 8 | ImmStringToInt(P.str);
  }
  else
  {
    return 0x2800 | GetRegisterCode(G.str) << 8 | GetRegisterCode(P.str);
  }
}

uint16_t Compiler::EncodeStoreInstruction(TokenLine::Token& G,
                                          TokenLine::Token& P)
{
  if (!is_operand(G) || !is_operand(G))
  {
    throw InvalidOperandException();
  }

  if (::isdigit(P.str[0]))
  {
    return 0x3000 | GetRegisterCode(G.str) << 8 | ImmStringToInt(P.str);
  }
  else
  {
    return 0x3800 | GetRegisterCode(G.str) << 8 | GetRegisterCode(P.str);   
  }
}

uint16_t Compiler::EncodeCallInstruction(TokenLine::Token& cond,
                                         TokenLine::Token& Imm)
{
  if (!is_operand(cond))
  {
    throw InvalidOperandException();
  }

  if (is_none(Imm))
  {
    return 0x8F00 | ImmStringToInt(cond.str);
  }

  if (!is_operand(Imm))
  {
    throw InvalidOperandException();
  }

  return 0x8F00 | GetConditionCode(cond.str) << 12 | ImmStringToInt(Imm.str);
}

uint16_t Compiler::EncodeJmpInstruction(TokenLine::Token& cond,
                                        TokenLine::Token& Imm)
{
  if (!is_operand(cond))
  {
    throw InvalidOperandException();
  }

  if (is_none(Imm))
  {
    return 0x8700 | ImmStringToInt(cond.str);
  }

  if (!is_operand(Imm))
  {
    throw InvalidOperandException();
  }

  return 0x8700 | GetConditionCode(cond.str) << 12 | ImmStringToInt(Imm.str);
}

uint16_t Compiler::EncodeMoviInstruction(
    TokenLine::Token& cond,
    TokenLine::Token& Gd, 
    TokenLine::Token& Imm)
{
  if (!is_operand(cond) || !is_operand(Gd))
  {
    throw InvalidInstructionException();
  }

  if (is_none(Imm))
  {
    return 0x8000 | GetRegisterCode(cond.str) << 8 | ImmStringToInt(Gd.str);
  }

  if (!is_operand(Imm))
  {
    throw InvalidOperandException();
  }

  return 0x8000 | GetConditionCode(cond.str) << 12
          | GetRegisterCode(Gd.str) << 8 | ImmStringToInt(Imm.str);
}

uint16_t Compiler::EncodeMovInstruction(TokenLine::Token& Gd,
                                        TokenLine::Token& Gs)
{
  if (!is_operand(Gd) || !is_operand(Gs))
  {
    throw InvalidInstructionException();
  }

  return 0x1800 | GetRegisterCode(Gd.str) << 8 | GetRegisterCode(Gs.str) << 4;
}

uint16_t Compiler::EncodeAluInstruction(
    TokenLine::Token& operation,
    TokenLine::Token& Gd,
    TokenLine::Token& Gs,
    TokenLine::Token& Op2)
{
  if (is_none(Op2))
  {
    return EncodeUnaryAluInstruction(operation, Gd, Gs);
  }

  return EncodeBinaryAluInstruction(operation, Gd, Gs, Op2);
}

uint16_t Compiler::EncodeBinaryAluInstruction(
    TokenLine::Token& operation,
    TokenLine::Token& Gd,
    TokenLine::Token& Gs,
    TokenLine::Token& Op2)
{
  if (!is_operand(Gd) || !is_operand(Gs)
      || !is_operand(Op2))
  {
    throw InvalidInstructionException();
  }

  uint16_t icode = 0x4000;

  if (operation.str == "adc");
  else if (operation.str == "add") icode |= 1 << 11;
  else if (operation.str == "sbc") icode |= 2 << 11;
  else if (operation.str == "sub") icode |= 3 << 11;
  else if (operation.str == "and") icode |= 4 << 11;
  else if (operation.str == "or") icode |= 5 << 11;
  else if (operation.str == "xor") icode |= 6 << 11;
  else throw InvalidInstructionException();

  if (Gd.str != "f")
    icode |= (GetRegisterCode(Gd.str) << 8) | 0x0008;
  
  icode |= GetRegisterCode(Gs.str) << 4;

  if (::isdigit(Op2.str[0]))
  {
    icode |= (ImmStringToInt(Op2.str) & 0x07) | 0x0080;
  }
  else
  {
    icode |= GetRegisterCode(Op2.str);
  }

  return icode;
}

uint16_t Compiler::EncodeUnaryAluInstruction(
    TokenLine::Token& operation,
    TokenLine::Token& Gd,
    TokenLine::Token& Gs)
{
  if (!is_operand(Gd) || !is_operand(Gs))
  {
    throw InvalidInstructionException();
  }

  uint16_t icode = 0x7800;

  if (operation.str == "not");
  else if (operation.str == "ror") icode |= 1 << 1;
  else if (operation.str == "shr") icode |= 2 << 1;
  else if (operation.str == "rcr") icode |= 3 << 1;
  else throw InvalidInstructionException();
  
  if (Gd.str != "f")
    icode |= (GetRegisterCode(Gd.str)) << 8 | 0x08;

  icode |= GetRegisterCode(Gs.str) << 4;

  return icode;
}

uint8_t Compiler::GetRegisterCode(const std::string& name)
{
  if (name == "a") return 0;
  else if (name == "b") return 1;
  else if (name == "c") return 2;
  else if (name == "d") return 3;
  else if (name == "m") return 4;
  else if (name == "s") return 5;
  else if (name == "l") return 6;
  else if (name == "pc") return 7;
  throw CompilerException("invalid register");
}

uint8_t Compiler::GetConditionCode(const std::string& name)
{
  if (name == "a") return 0;
  else if (name == "z") return 1;
  else if (name == "ns") return 2;
  else if (name == "c") return 3;
  else if (name == "nc") return 4;
  else if (name == "s") return 5;
  else if (name == "nz") return 6;
  throw CompilerException("invalid condition");
}

inline uint8_t Compiler::ImmStringToInt(const std::string& Imm)
{
  if (!Imm.compare(0, 2, "0x"))
  {
    return std::stoi(Imm, nullptr, 16);
  }
  else if (strisdigit(Imm))
  {
    return std::stoi(Imm, nullptr, 10);
  }
  throw CompilerException("invalid immediate value");
}
