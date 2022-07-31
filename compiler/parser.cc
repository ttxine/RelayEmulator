#include <stdexcept>

#include "compiler/parser.h"
#include "utils/str.h"

Parser::Parser(std::vector<std::pair<Token, std::string>> tokens)
{
  tokens_ = tokens;
  cur_token_ = tokens_.begin();

  InitializeInstructions();
}

std::unique_ptr<Root> Parser::Parse()
{
  std::unique_ptr<Root> root(new Root);

  while (cur_token_ < tokens_.end())
  {
    root->nodes.push_back(ParseNextNode());
  }

  return root;
}

std::unique_ptr<Node> Parser::ParseNextNode()
{
  std::pair<Token, std::string> token = *cur_token_;

  switch (token.first)
  {
    case Token::kLabel:
      return ParseLabel();
    case Token::kInstruction:
      return ParseInstruction();
    default:
      throw std::runtime_error("instruction or label expected");
  }
}

std::unique_ptr<LabelNode> Parser::ParseLabel()
{
  labels_[cur_token_->second] = cur_addr_;

  std::unique_ptr<LabelNode> label(new LabelNode);
  label->str = cur_token_->second;

  ++cur_token_;
  return label;
}

std::unique_ptr<OperandNode> Parser::ParseOperand()
{
  std::unique_ptr<OperandNode> operand(new OperandNode);
  operand->str = cur_token_->second;
  operand->type = cur_token_->first;

  ++cur_token_;
  return operand;
}

std::unique_ptr<InstructionNode> Parser::ParseInstruction()
{
  std::unique_ptr<InstructionNode> instruction(new InstructionNode);
  instruction->str = cur_token_->second;

  ++cur_token_;
  switch (GetInstructionLength(instruction->str))
  {
    case InstructionLength::kNoOperands:
    {
      break;
    }
    case InstructionLength::kTwoOperands:
    {
      instruction->operands = TakeTwoOperands();
      break;
    }
    case InstructionLength::kThreeOperands:
    {
      instruction->operands = TakeThreeOperands();
      break;
    }
    case InstructionLength::kTwoOperandsOptional:
    {
      instruction->operands = TakeTwoOperandsOptional();
      break;
    }
    case InstructionLength::kThreeOperandsOptional:
    {
      instruction->operands = TakeThreeOperandsOptional();
      break;
    }
  }

  ++cur_addr_;
  return instruction;
}

Parser::InstructionLength Parser::GetInstructionLength(
    const std::string& instruction)
{
  auto iter = instructions_.find(strtolower(instruction));

  if (iter != instructions_.end())
  {
    return (*iter).second;
  }

  throw std::runtime_error("invalid instruction");
};

std::vector<std::unique_ptr<OperandNode>> Parser::TakeTwoOperands()
{
  std::vector<std::unique_ptr<OperandNode>> operands;

  if (IsTokenOperand(GetCurrentToken().first))
  {
    operands.push_back(ParseOperand());
  }
  else
  {
    throw std::runtime_error("operand expected");
  }
  
  if (GetCurrentToken().first == Token::kComma)
  {
    ++cur_token_;
  }
  else
  {
    throw std::runtime_error("comma expected");
  }

  if (IsTokenOperand(GetCurrentToken().first))
  {
    operands.push_back(ParseOperand());
  }
  else
  {
    throw std::runtime_error("operand expected");
  }

  return operands;
}

std::vector<std::unique_ptr<OperandNode>> Parser::TakeThreeOperands()
{
  std::vector<std::unique_ptr<OperandNode>> operands;

  if (IsTokenOperand(GetCurrentToken().first))
  {
    operands.push_back(ParseOperand());
  }
  else
  {
    throw std::runtime_error("operand expected");
  }

  if (GetCurrentToken().first == Token::kComma)
  {
    ++cur_token_;
  }
  else
  {
    throw std::runtime_error("comma expected");
  }

  if (IsTokenOperand(GetCurrentToken().first))
  {
    operands.push_back(ParseOperand());
  }
  else
  {
    throw std::runtime_error("operand expected");
  }

  if (GetCurrentToken().first == Token::kComma)
  {
    ++cur_token_;
  }
  else
  {
    throw std::runtime_error("comma expected");
  }

  if (IsTokenOperand(GetCurrentToken().first))
  {
    operands.push_back(ParseOperand());
  }
  else
  {
    throw std::runtime_error("operand expected");
  }

  return operands;
}

std::vector<std::unique_ptr<OperandNode>> Parser::TakeTwoOperandsOptional()
{
  std::vector<std::unique_ptr<OperandNode>> operands;

  if (IsTokenOperand(GetCurrentToken().first))
  {
    operands.push_back(ParseOperand());
  }
  else
  {
    throw std::runtime_error("operand expected");
  }

  if (GetCurrentToken().first == Token::kComma)
  {
    ++cur_token_;
  }
  else
  {
    return operands;
  }

  if (IsTokenOperand(GetCurrentToken().first))
  {
    operands.push_back(ParseOperand());
  }
  else
  {
    throw std::runtime_error("operand expected");
  }

  return operands;
}

std::vector<std::unique_ptr<OperandNode>> Parser::TakeThreeOperandsOptional()
{
  std::vector<std::unique_ptr<OperandNode>> operands;

  if (IsTokenOperand(GetCurrentToken().first))
  {
    operands.push_back(ParseOperand());
  }
  else
  {
    throw std::runtime_error("operand expected");
  }

  if (GetCurrentToken().first == Token::kComma)
  {
    ++cur_token_;
  }
  else
  {
    throw std::runtime_error("comma expected");
  }

  if (IsTokenOperand(GetCurrentToken().first))
  {
    operands.push_back(ParseOperand());
  }
  else
  {
    throw std::runtime_error("operand expected");
  }

  if (GetCurrentToken().first == Token::kComma)
  {
    ++cur_token_;
  }
  else
  {
    return operands;
  }

  if (IsTokenOperand(GetCurrentToken().first))
  {
    operands.push_back(ParseOperand());
  }
  else
  {
    throw std::runtime_error("operand expected");
  }

  return operands;
}

void Parser::InitializeInstructions()
{
  instructions_["halt"] = InstructionLength::kNoOperands;
  instructions_["nop"] = InstructionLength::kNoOperands;
  instructions_["load"] = InstructionLength::kTwoOperands;
  instructions_["store"] = InstructionLength::kTwoOperands;
  instructions_["load"] = InstructionLength::kTwoOperands;
  instructions_["jmp"] = InstructionLength::kTwoOperandsOptional;
  instructions_["movi"] = InstructionLength::kThreeOperandsOptional;
  instructions_["mov"] = InstructionLength::kTwoOperands;
  instructions_["adc"] = InstructionLength::kThreeOperands;
  instructions_["add"] = InstructionLength::kThreeOperands;
  instructions_["sbc"] = InstructionLength::kThreeOperands;
  instructions_["sub"] = InstructionLength::kThreeOperands;
  instructions_["and"] = InstructionLength::kThreeOperands;
  instructions_["or"] = InstructionLength::kThreeOperands;
  instructions_["xor"] = InstructionLength::kThreeOperands;
  instructions_["not"] = InstructionLength::kTwoOperands;
  instructions_["ror"] = InstructionLength::kTwoOperands;
  instructions_["shr"] = InstructionLength::kTwoOperands;
  instructions_["rcr"] = InstructionLength::kTwoOperands;
}
