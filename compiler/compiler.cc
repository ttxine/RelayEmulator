#include <memory>
#include <stdexcept>

#include "compiler/compiler.h"
#include "utils/str.h"

TemporaryFile Compiler::Compile() const
try
{
  TemporaryFile file;

  for (Node node : root_)
  {
    if (IsInstruction(node))
    {
      uint16_t opcode = AssembleInstruction(node);

      file.Write(static_cast<uint8_t>(opcode >> 8));
      file.Write(static_cast<uint8_t>(opcode));
    }
    else if (!IsLabel(node))
    {
      throw std::runtime_error("instruction or label expected");
    }
  }

  return file;
}
catch (const std::runtime_error&)
{
  throw;
}

uint16_t Compiler::AssembleInstruction(const Node& node) const
{
  std::string str = strtolower(node.GetString());

  if (str == "halt") return AssembleHALT();
  else if (str == "nop") return AssembleNOP();
  else if (str == "load") return AssembleLOAD(node);
  else if (str == "store") return AssembleSTORE(node);
  else if (str == "call") return AssembleCALL(node);
  else if (str == "jmp") return AssembleJMP(node);
  else if (str == "movi") return AssembleMOVI(node);
  else if (str == "mov") return AssembleMOV(node);
  else if (str == "adc") return AssembleADC(node);
  else if (str == "add") return AssembleADD(node);
  else if (str == "sbc") return AssembleSBC(node);
  else if (str == "sub") return AssembleSUB(node);
  else if (str == "and") return AssembleAND(node);
  else if (str == "or") return AssembleOR(node);
  else if (str == "xor") return AssembleXOR(node);
  else if (str == "not") return AssembleNOT(node);
  else if (str == "ror") return AssembleROR(node);
  else if (str == "shr") return AssembleSHR(node);
  else if (str == "rcr") return AssembleRCR(node);
  else throw std::runtime_error("invalid instruction");
}

inline uint16_t Compiler::AssembleHALT() const
{
  return 0x1000;
}

inline uint16_t Compiler::AssembleNOP() const
{
  return 0x0000;
}

uint16_t Compiler::AssembleLOAD(const Node& node) const
{
  if (IsOperandRegister(node.GetSubNodes()[0]) &&
      IsOperandRegister(node.GetSubNodes()[1]))
  {
    uint8_t G = GetRegisterCode(node.GetSubNodes()[0].GetString());
    uint8_t P = GetRegisterCode(node.GetSubNodes()[1].GetString());

    return 0x2800 | G << 8 | P;
  }
  else if (IsOperandRegister(node.GetSubNodes()[0]) &&
           IsOperandNumerical(node.GetSubNodes()[1]))
  {
    uint8_t G = GetRegisterCode(node.GetSubNodes()[0].GetString());
    uint8_t Imm = asm_stoi(node.GetSubNodes()[1].GetString());

    return 0x2000 | G << 8 | Imm;
  }
  else
  {
    throw std::runtime_error("invalid combination of opcode and operands");
  }
}

uint16_t Compiler::AssembleSTORE(const Node& node) const
{
  if (IsOperandRegister(node.GetSubNodes()[0]) &&
      IsOperandRegister(node.GetSubNodes()[1]))
  {
    uint8_t G = GetRegisterCode(node.GetSubNodes()[0].GetString());
    uint8_t P = GetRegisterCode(node.GetSubNodes()[1].GetString());

    return 0x3800 | G << 8 | P;
  }
  else if (IsOperandRegister(node.GetSubNodes()[0]) &&
           IsOperandNumerical(node.GetSubNodes()[1]))
  {
    uint8_t G = GetRegisterCode(node.GetSubNodes()[0].GetString());
    uint8_t Imm = asm_stoi(node.GetSubNodes()[1].GetString());

    return 0x3000 | G << 8 | Imm;
  }
  else
  {
    throw std::runtime_error("invalid combination of opcode and operands");
  }
}

uint16_t Compiler::AssembleCALL(const Node& node) const
{
  const bool bNodeHasTwoOperands = node.GetSubNodes().size() == 2;

  if (bNodeHasTwoOperands && IsOperandCondition(node.GetSubNodes()[0]) &&
      IsOperandNumerical(node.GetSubNodes()[1]))
  {
    uint8_t Cond = GetConditionCode(node.GetSubNodes()[0].GetString());
    uint8_t Imm = asm_stoi(node.GetSubNodes()[1].GetString());

    return 0x8F00 | Cond << 12 | Imm;
  }
  else if (bNodeHasTwoOperands && IsOperandCondition(node.GetSubNodes()[0]) &&
           IsOperandIdentifier(node.GetSubNodes()[1]))
  {
    auto label = labels_.find(node.GetSubNodes()[1].GetString());

    if (label != labels_.end())
    {
      uint8_t Cond = GetConditionCode(node.GetSubNodes()[0].GetString());
      uint8_t Imm = label->second;

      return 0x8F00 | Cond << 12 | Imm;
    }
    else
    {
      throw std::runtime_error("unknown identifier: \"" +
                               node.GetSubNodes()[0].GetString() + '\"');
    }
  }
  else if (IsOperandNumerical(node.GetSubNodes()[0]))
  {
    uint8_t Imm = asm_stoi(node.GetSubNodes()[0].GetString());

    return 0x8F00 | Imm;
  }
  else if (IsOperandIdentifier(node.GetSubNodes()[0]))
  {
    auto label = labels_.find(node.GetSubNodes()[0].GetString());

    if (label != labels_.end())
    {
      uint8_t Imm = label->second;

      return 0x8F00 | Imm;
    }
    else
    {
      throw std::runtime_error("unknown identifier: \"" +
                               node.GetSubNodes()[0].GetString() + '\"');
    }
  }
  else
  {
    throw std::runtime_error("invalid combination of opcode and operands");
  }
}

uint16_t Compiler::AssembleJMP(const Node& node) const
{
  const bool bNodeHasTwoOperands = node.GetSubNodes().size() == 2;

  if (bNodeHasTwoOperands && IsOperandCondition(node.GetSubNodes()[0]) &&
      IsOperandNumerical(node.GetSubNodes()[1]))
  {
    uint8_t Cond = GetConditionCode(node.GetSubNodes()[0].GetString());
    uint8_t Imm = asm_stoi(node.GetSubNodes()[1].GetString());

    return 0x8700 | Cond << 12 | Imm;
  }
  else if (bNodeHasTwoOperands && IsOperandCondition(node.GetSubNodes()[0]) &&
           IsOperandIdentifier(node.GetSubNodes()[1]))
  {
    auto label = labels_.find(node.GetSubNodes()[1].GetString());

    if (label != labels_.end())
    {
      uint8_t Cond = GetConditionCode(node.GetSubNodes()[0].GetString());
      uint8_t Imm = label->second;

      return 0x8700 | Cond << 12 | Imm;
    }
    else
    {
      throw std::runtime_error("unknown identifier: \"" +
                               node.GetSubNodes()[0].GetString() + '\"');
    }
  }
  else if (IsOperandNumerical(node.GetSubNodes()[0]))
  {
    uint8_t Imm = asm_stoi(node.GetSubNodes()[0].GetString());

    return 0x8700 | Imm;
  }
  else if (IsOperandIdentifier(node.GetSubNodes()[0]))
  {
    auto label = labels_.find(node.GetSubNodes()[0].GetString());

    if (label != labels_.end())
    {
      uint8_t Imm = label->second;

      return 0x8700 | Imm;
    }
    else
    {
      throw std::runtime_error("unknown identifier: \"" +
                               node.GetSubNodes()[0].GetString() + '\"');
    }
  }
  else
  {
    throw std::runtime_error("invalid combination of opcode and operands");
  }
}

uint16_t Compiler::AssembleMOVI(const Node& node) const
{
  const bool bNodeHasThreeOperands = node.GetSubNodes().size() == 3;

  if (bNodeHasThreeOperands && IsOperandCondition(node.GetSubNodes()[0]) &&
      IsOperandRegister(node.GetSubNodes()[1]) &&
      IsOperandNumerical(node.GetSubNodes()[2]))
  {
    uint8_t Cond = GetConditionCode(node.GetSubNodes()[0].GetString());
    uint8_t Gd = GetRegisterCode(node.GetSubNodes()[1].GetString());
    uint8_t Imm = asm_stoi(node.GetSubNodes()[2].GetString());

    return 0x8000 | Cond << 12 | Gd << 8 | Imm;
  }
  else if (IsOperandRegister(node.GetSubNodes()[0]) &&
           IsOperandNumerical(node.GetSubNodes()[1]))
  {
    uint8_t Gd = GetRegisterCode(node.GetSubNodes()[1].GetString());
    uint8_t Imm = asm_stoi(node.GetSubNodes()[2].GetString());

    return 0x8000 | Gd << 8 | Imm;
  }
  else
  {
    throw std::runtime_error("invalid combination of opcode and operands");
  }
}

uint16_t Compiler::AssembleMOV(const Node& node) const
{
  if (IsOperandRegister(node.GetSubNodes()[0]) &&
      IsOperandRegister(node.GetSubNodes()[1]))
  {
    uint8_t Gd = GetRegisterCode(node.GetSubNodes()[0].GetString());
    uint8_t Gs = GetRegisterCode(node.GetSubNodes()[1].GetString());

    return 0x1800 | Gd << 8 | Gs << 4;
  }
  else
  {
    throw std::runtime_error("invalid combination of opcode and operands");
  }
}

uint16_t Compiler::AssembleADC(const Node& node) const
{
  return AssembleBinaryALU(node, 0);
}

uint16_t Compiler::AssembleADD(const Node& node) const
{
  return AssembleBinaryALU(node, 1);
}

uint16_t Compiler::AssembleSBC(const Node& node) const
{
  return AssembleBinaryALU(node, 2);
}

uint16_t Compiler::AssembleSUB(const Node& node) const
{
  return AssembleBinaryALU(node, 3);
}

uint16_t Compiler::AssembleAND(const Node& node) const
{
  return AssembleBinaryALU(node, 4);
}

uint16_t Compiler::AssembleOR(const Node& node) const
{
  return AssembleBinaryALU(node, 5);
}

uint16_t Compiler::AssembleXOR(const Node& node) const
{
  return AssembleBinaryALU(node, 6);
}

uint16_t Compiler::AssembleNOT(const Node& node) const
{
  return AssembleUnaryALU(node, 0);
}

uint16_t Compiler::AssembleROR(const Node& node) const
{
  return AssembleUnaryALU(node, 1);
}

uint16_t Compiler::AssembleSHR(const Node& node) const
{
  return AssembleUnaryALU(node, 2);
}

uint16_t Compiler::AssembleRCR(const Node& node) const
{
  return AssembleUnaryALU(node, 3);
}

uint16_t Compiler::AssembleBinaryALU(const Node& node,
                                     uint8_t code) const
{
  if (IsOperandRegister(node.GetSubNodes()[0]) &&
      IsOperandRegister(node.GetSubNodes()[1]) &&
      IsOperandRegister(node.GetSubNodes()[2]))
  {
    bool r = strtolower(node.GetSubNodes()[0].GetString()) != "f";

    uint8_t Gd = r ? GetRegisterCode(node.GetSubNodes()[0].GetString()) : 0;
    uint8_t Gs1 = GetRegisterCode(node.GetSubNodes()[1].GetString());
    uint8_t Op2 = GetRegisterCode(node.GetSubNodes()[2].GetString());

    return 0x4000 | code << 11 | Gd << 8 | Gs1 << 4 | r << 3 | Op2;
  }
  else if (IsOperandRegister(node.GetSubNodes()[0]) &&
           IsOperandRegister(node.GetSubNodes()[1]) &&
           IsOperandNumerical(node.GetSubNodes()[2]))
  {
    bool r = strtolower(node.GetSubNodes()[0].GetString()) != "f";

    uint8_t Gd = r ? GetRegisterCode(node.GetSubNodes()[0].GetString()) : 0;
    uint8_t Gs1 = GetRegisterCode(node.GetSubNodes()[1].GetString());
    uint8_t Op2 = asm_stoi(node.GetSubNodes()[2].GetString());

    return 0x4000 | code << 11 | Gd << 8 | 1 << 7 | Gs1 << 4 | r << 3 | Op2;
  }
  else
  {
    throw std::runtime_error("invalid combination of opcode and operands");
  }
}

uint16_t Compiler::AssembleUnaryALU(const Node& node,
                                     uint8_t code) const
{
  if (IsOperandRegister(node.GetSubNodes()[0]) &&
      IsOperandRegister(node.GetSubNodes()[1]))
  {
    bool r = strtolower(node.GetSubNodes()[0].GetString()) != "f";

    uint8_t Gd = r ? GetRegisterCode(node.GetSubNodes()[0].GetString()) : 0;
    uint8_t Gs = GetRegisterCode(node.GetSubNodes()[1].GetString());

    return 0x7800 | Gd << 8 | Gs << 4 | r << 3 | code << 1;
  }
  else
  {
    throw std::runtime_error("invalid combination of opcode and operands");
  }
}

uint8_t Compiler::GetRegisterCode(const std::string& str) const
{
  std::string name = strtolower(str);

  if (name == "a") return 0;
  else if (name == "b") return 1;
  else if (name == "c") return 2;
  else if (name == "d") return 3;
  else if (name == "m") return 4;
  else if (name == "s") return 5;
  else if (name == "l") return 6;
  else if (name == "pc") return 7;
  else throw std::runtime_error("invalid register");
}

uint8_t Compiler::GetConditionCode(const std::string& str) const
{
  std::string name = strtolower(str);

  if (name == "a") return 0;
  else if (name == "z") return 1;
  else if (name == "ns") return 2;
  else if (name == "c") return 3;
  else if (name == "nc") return 4;
  else if (name == "s") return 5;
  else if (name == "nz") return 6;
  else throw std::runtime_error("invalid condition");
}
