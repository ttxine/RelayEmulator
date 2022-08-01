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
    std::unique_ptr<Root> Parse();

    const std::unordered_map<std::string, int> GetLabels() const
    {
      return labels_;
    }

  private:
    std::unique_ptr<Node> ParseNextNode();
    std::unique_ptr<Node> ParseLabel();
    std::unique_ptr<Node> ParseInstruction();
    std::unique_ptr<Node> ParseOperand();

    std::vector<std::unique_ptr<Node>> TakeOneOperand();
    std::vector<std::unique_ptr<Node>> TakeTwoOperands();
    std::vector<std::unique_ptr<Node>> TakeThreeOperands();
    std::vector<std::unique_ptr<Node>> TakeTwoOperandsOptional();
    std::vector<std::unique_ptr<Node>> TakeThreeOperandsOptional();

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
