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
    std::shared_ptr<Node> ParseNextNode();
    std::shared_ptr<LabelNode> ParseLabel();
    std::shared_ptr<InstructionNode> ParseInstruction();
    std::shared_ptr<OperandNode> ParseOperand();

    void InitializeInstructions();
    InstructionLength GetInstructionLength(const std::string& instruction);

    std::vector<std::shared_ptr<OperandNode>> TakeOneOperand();
    std::vector<std::shared_ptr<OperandNode>> TakeTwoOperands();
    std::vector<std::shared_ptr<OperandNode>> TakeThreeOperands();
    std::vector<std::shared_ptr<OperandNode>> TakeTwoOperandsOptional();
    std::vector<std::shared_ptr<OperandNode>> TakeThreeOperandsOptional();

    std::pair<Token, std::string> GetCurrentToken() const
    {
      return *cur_token_;
    }

    bool IsOperand(Token token) const
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
