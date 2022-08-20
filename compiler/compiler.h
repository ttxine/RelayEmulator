#pragma once
#include <unordered_map>

#include "compiler/ast.h"
#include "utils/str.h"
#include "utils/tempfile.h"

class Compiler
{
  public:
    Compiler(const Root& root,
             const std::unordered_map<std::string, int>& labels)
        : root_(root), labels_(labels)
    {
    }

  public:
    TemporaryFile Compile() const;

  private:
    uint16_t AssembleInstruction(const Node& node) const;
    uint8_t GetRegisterCode(const std::string& str) const;
    uint8_t GetConditionCode(const std::string& str) const;

    bool IsInstruction(const Node& node) const
    {
      return node.GetTokenType() == Token::kInstruction;
    }

    bool IsLabel(const Node& node) const
    {
      return node.GetTokenType() == Token::kLabel;
    }

    bool IsOperandCondition(const Node& op) const
    {
      return op.GetTokenType() == Token::kCondition ||
             (op.GetTokenType() == Token::kRegister &&
             strtolower(op.GetString()) == "a");
    }

    bool IsOperandNumerical(const Node& op) const
    {
      return op.GetTokenType() == Token::kNumerical;
    }

    bool IsOperandRegister(const Node& op) const
    {
      return op.GetTokenType() == Token::kRegister;
    }

    bool IsOperandIdentifier(const Node& op) const
    {
      return op.GetTokenType() == Token::kIdentifier;
    }

  private:
    uint16_t AssembleHALT() const;
    uint16_t AssembleNOP() const;
    uint16_t AssembleLOAD(const Node& node) const;
    uint16_t AssembleSTORE(const Node& node) const;
    uint16_t AssembleJMP(const Node& node) const;
    uint16_t AssembleCALL(const Node& node) const;
    uint16_t AssembleMOVI(const Node& node) const;
    uint16_t AssembleMOV(const Node& node) const;
    uint16_t AssembleADC(const Node& node) const;
    uint16_t AssembleADD(const Node& node) const;
    uint16_t AssembleSUB(const Node& node) const;
    uint16_t AssembleSBC(const Node& node) const;
    uint16_t AssembleAND(const Node& node) const;
    uint16_t AssembleOR(const Node& node) const;
    uint16_t AssembleXOR(const Node& node) const;
    uint16_t AssembleNOT(const Node& node) const;
    uint16_t AssembleROR(const Node& node) const;
    uint16_t AssembleSHR(const Node& node) const;
    uint16_t AssembleRCR(const Node& node) const;

  private:
    uint16_t AssembleBinaryALU(const Node& node, uint8_t code) const;
    uint16_t AssembleUnaryALU(const Node& node, uint8_t code) const;

  private:
    Root root_;
    std::unordered_map<std::string, int> labels_;
};
