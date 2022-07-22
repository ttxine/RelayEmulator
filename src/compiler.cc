#include <fstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>

#include "src/compiler.h"

std::string Compiler::Run()
{
  std::ifstream in(to_compile_);

  if (in.fail())
  {
    throw std::runtime_error("Can't open a file: \"" + to_compile_ + '"');
  }

  std::string out_path = "/tmp/RelayEmulatorCompiler~XXXXXX";
  int out_fd = mkstemp(&out_path[0]);

  std::string line;

  while (std::getline(in, line))
  {
    try
    {
      uint16_t icode = EncodeInstruction(line);

      uint8_t buf[2] = { static_cast<uint8_t>(icode >> 8),
                         static_cast<uint8_t>(icode) };

      ::write(out_fd, buf, sizeof(buf));
    }
    catch (CompilerException& e)
    {
      ::unlink(out_path.c_str());
      ::close(out_fd);
  
      std::cout << e.what() << " at line: \"" << line << "\"." << std::endl;
      ::exit(1);
    }
  }

  return out_path;
}

uint16_t Compiler::EncodeInstruction(const std::string& instruction)
{
  token::TokenLine tline(token::strtolower(instruction));

  token::TokenLine::Token itoken = tline.GetNextToken();

  if (!token::is_intruction(itoken))
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
    token::TokenLine::Token G = tline.GetNextToken();
    token::TokenLine::Token P = tline.GetNextToken();

    return EncodeLoadInstruction(G, P);
  }
  else if (itoken.str == "store")
  {
    token::TokenLine::Token G = tline.GetNextToken();
    token::TokenLine::Token P = tline.GetNextToken();

    return EncodeStoreInstruction(G, P);
  }
  else if (itoken.str == "call")
  {
    token::TokenLine::Token cond = tline.GetNextToken();
    token::TokenLine::Token Imm = tline.GetNextToken();

    return EncodeCallInstruction(cond, Imm);
  }
  else if (itoken.str == "jmp")
  {
    token::TokenLine::Token cond = tline.GetNextToken();
    token::TokenLine::Token Imm = tline.GetNextToken();

    return EncodeJmpInstruction(cond, Imm);
  }
  else if (itoken.str == "movi")
  {
    token::TokenLine::Token cond = tline.GetNextToken();
    token::TokenLine::Token Gd = tline.GetNextToken();
    token::TokenLine::Token Imm = tline.GetNextToken();

    return EncodeMoviInstruction(cond, Gd, Imm);
  }
  else if (itoken.str == "mov")
  {
    token::TokenLine::Token Gd = tline.GetNextToken();
    token::TokenLine::Token Gs = tline.GetNextToken();

    return EncodeMovInstruction(Gd, Gs);
  }
  else
  {
    token::TokenLine::Token Gd = tline.GetNextToken();
    token::TokenLine::Token Gs = tline.GetNextToken();
    token::TokenLine::Token Op2 = tline.GetNextToken();

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

uint16_t Compiler::EncodeLoadInstruction(token::TokenLine::Token& G,
                                         token::TokenLine::Token& P)
{
  if (!token::is_operand(G) || !token::is_operand(G))
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

uint16_t Compiler::EncodeStoreInstruction(token::TokenLine::Token& G,
                                          token::TokenLine::Token& P)
{
  if (!token::is_operand(G) || !token::is_operand(G))
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

uint16_t Compiler::EncodeCallInstruction(token::TokenLine::Token& cond,
                                         token::TokenLine::Token& Imm)
{
  if (!token::is_operand(cond))
  {
    throw InvalidOperandException();
  }

  if (token::is_none(Imm))
  {
    return 0x8F00 | ImmStringToInt(cond.str);
  }

  if (!token::is_operand(Imm))
  {
    throw InvalidOperandException();
  }

  return 0x8F00 | GetConditionCode(cond.str) << 12 | ImmStringToInt(Imm.str);
}

uint16_t Compiler::EncodeJmpInstruction(token::TokenLine::Token& cond,
                                        token::TokenLine::Token& Imm)
{
  if (!token::is_operand(cond))
  {
    throw InvalidOperandException();
  }

  if (token::is_none(Imm))
  {
    return 0x8700 | ImmStringToInt(cond.str);
  }

  if (!token::is_operand(Imm))
  {
    throw InvalidOperandException();
  }

  return 0x8700 | GetConditionCode(cond.str) << 12 | ImmStringToInt(Imm.str);
}

uint16_t Compiler::EncodeMoviInstruction(
    token::TokenLine::Token& cond,
    token::TokenLine::Token& Gd, 
    token::TokenLine::Token& Imm)
{
  if (!token::is_operand(cond) || !token::is_operand(Gd))
  {
    throw InvalidInstructionException();
  }

  if (token::is_none(Imm))
  {
    return 0x8000 | GetRegisterCode(cond.str) << 8 | ImmStringToInt(Gd.str);
  }

  if (!token::is_operand(Imm))
  {
    throw InvalidOperandException();
  }

  return 0x8000 | GetConditionCode(cond.str) << 12
          | GetRegisterCode(Gd.str) << 8 | ImmStringToInt(Imm.str);
}

uint16_t Compiler::EncodeMovInstruction(token::TokenLine::Token& Gd,
                                        token::TokenLine::Token& Gs)
{
  if (!token::is_operand(Gd) || !token::is_operand(Gs))
  {
    throw InvalidInstructionException();
  }

  return 0x1800 | GetRegisterCode(Gd.str) << 8 | GetRegisterCode(Gs.str) << 4;
}

uint16_t Compiler::EncodeAluInstruction(
    token::TokenLine::Token& operation,
    token::TokenLine::Token& Gd,
    token::TokenLine::Token& Gs,
    token::TokenLine::Token& Op2)
{
  if (token::is_none(Op2))
  {
    return EncodeUnaryAluInstruction(operation, Gd, Gs);
  }

  return EncodeBinaryAluInstruction(operation, Gd, Gs, Op2);
}

uint16_t Compiler::EncodeBinaryAluInstruction(
    token::TokenLine::Token& operation,
    token::TokenLine::Token& Gd,
    token::TokenLine::Token& Gs,
    token::TokenLine::Token& Op2)
{
  if (!token::is_operand(Gd) || !token::is_operand(Gs)
      || !token::is_operand(Op2))
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

  if (Gd.str != "F")
    icode |= (GetRegisterCode(Gd.str) << 8) | 0x0008;
  
  icode |= GetRegisterCode(Gs.str) << 4;

  if (::isdigit(Op2.str[0]))
  try
  {
    icode |= GetRegisterCode(Op2.str);
  }
  catch (CompilerException& e)
  {
    icode |= (ImmStringToInt(Op2.str) & 0x07) | 0x0080;
  }

  return icode;
}

uint16_t Compiler::EncodeUnaryAluInstruction(
    token::TokenLine::Token& operation,
    token::TokenLine::Token& Gd,
    token::TokenLine::Token& Gs)
{
  if (!token::is_operand(Gd) || !token::is_operand(Gs))
  {
    throw InvalidInstructionException();
  }

  uint16_t icode = 0x7800;

  if (operation.str == "not");
  else if (operation.str == "ror") icode |= 1 << 1;
  else if (operation.str == "shr") icode |= 2 << 1;
  else if (operation.str == "rcr") icode |= 3 << 1;
  else throw InvalidInstructionException();
  
  if (Gd.str != "F")
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
  throw CompilerException("Invalid register");
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
  throw CompilerException("Invalid condition");
}

inline uint8_t Compiler::ImmStringToInt(const std::string& Imm)
{
  if (!Imm.compare(0, 2, "0x"))
  {
    return std::stoi(Imm, nullptr, 16);
  }
  else
  {
    return std::stoi(Imm, nullptr, 10);
  }
}
