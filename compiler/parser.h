#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "compiler/ast.h"

class Parser
{
  public:
    enum class InstructionLength
    {
      kNoOperands,
      kTwoOperands,
      kTwoOperandsOptional,
      kThreeOperands,
      kThreeOperandsOptional
    };

  public:
    Parser(std::vector<std::pair<Token, std::string>> tokens);

  public:
    Root Parse();

    const std::unordered_map<std::string, int> GetLabels() const
    {
      return labels_;
    }

  private:
    Node ParseNextNode();
    Node ParseLabel();
    Node ParseInstruction();
    Node ParseOperand();

    std::vector<Node> TakeOneOperand();
    std::vector<Node> TakeTwoOperands();
    std::vector<Node> TakeThreeOperands();
    std::vector<Node> TakeTwoOperandsOptional();
    std::vector<Node> TakeThreeOperandsOptional();

    void InitializeInstructions();
    InstructionLength GetInstructionLength(const std::string& instruction);

    std::pair<Token, std::string> GetCurrentToken() const
    {
      return *cur_token_;
    }

    bool IsTokenOperand(Token token) const
    {
      return token == Token::kNumerical || token == Token::kRegister ||
             token == Token::kIdentifier || token == Token::kCondition;
    }

  private:
    std::unordered_map<std::string, int> labels_;
    std::unordered_map<std::string, InstructionLength> instructions_;

    std::vector<std::pair<Token, std::string>> tokens_;
    std::vector<std::pair<Token, std::string>>::const_iterator cur_token_;

    int cur_addr_ = 0;
};
