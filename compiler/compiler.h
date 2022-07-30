#pragma once
#include <unordered_map>

#include "compiler/ast.h"
#include "utils/str.h"

class Compiler
{
  public:
    Compiler(const Root& root,
             const std::unordered_map<std::string, int>& labels)
        : root_(root), labels_(labels)
    {
    }

  public:
    std::string Compile() const;

  private:
    uint16_t AssembleInstruction(const InstructionNode& node) const;
    uint8_t GetRegisterCode(const std::string& str) const;
    uint8_t GetConditionCode(const std::string& str) const;

    bool IsOperandCondition(const OperandNode& op) const
    {
      return op.type == Token::kCondition || (op.type == Token::kRegister
             && strtolower(op.str) == "a");
    }

    bool IsOperandNumerical(const OperandNode& op) const
    {
      return op.type == Token::kNumerical;
    }

    bool IsOperandRegister(const OperandNode& op) const
    {
      return op.type == Token::kRegister;
    }

    bool IsOperandIdentifier(const OperandNode& op) const
    {
      return op.type == Token::kIdentifier;
    }

  private:
    uint16_t AssembleHALT() const;
    uint16_t AssembleNOP() const;
    uint16_t AssembleLOAD(const InstructionNode& node) const;
    uint16_t AssembleSTORE(const InstructionNode& node) const;
    uint16_t AssembleJMP(const InstructionNode& node) const;
    uint16_t AssembleCALL(const InstructionNode& node) const;
    uint16_t AssembleMOVI(const InstructionNode& node) const;
    uint16_t AssembleMOV(const InstructionNode& node) const;
    uint16_t AssembleADC(const InstructionNode& node) const;
    uint16_t AssembleADD(const InstructionNode& node) const;
    uint16_t AssembleSUB(const InstructionNode& node) const;
    uint16_t AssembleSBC(const InstructionNode& node) const;
    uint16_t AssembleAND(const InstructionNode& node) const;
    uint16_t AssembleOR(const InstructionNode& node) const;
    uint16_t AssembleXOR(const InstructionNode& node) const;
    uint16_t AssembleNOT(const InstructionNode& node) const;
    uint16_t AssembleROR(const InstructionNode& node) const;
    uint16_t AssembleSHR(const InstructionNode& node) const;
    uint16_t AssembleRCR(const InstructionNode& node) const;

  private:
    uint16_t AssembleBinaryALU(const InstructionNode& node, uint8_t code) const;
    uint16_t AssembleUnaryALU(const InstructionNode& node, uint8_t code) const;

  private:
    Root root_;
    std::unordered_map<std::string, int> labels_;
};
