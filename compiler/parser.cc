#include <stdexcept>

#include "compiler/parser.h"
#include "utils/str.h"

Parser::Parser(std::vector<std::pair<Token, std::string>> tokens)
{
  tokens_ = tokens;
  cur_token_ = tokens_.begin();

  InitializeInstructions();
}

Root Parser::Parse()
{
  Root root;

  while (cur_token_ < tokens_.end())
  {
    root.push_back(ParseNextNode());
  }

  return root;
}

Node Parser::ParseNextNode()
{
  switch (cur_token_->first)
  {
    case Token::kLabel:
      return ParseLabel();
    case Token::kInstruction:
      return ParseInstruction();
    default:
      throw std::runtime_error("instruction or label expected");
  }
}

Node Parser::ParseLabel()
{
  labels_[cur_token_->second] = cur_addr_;
  Node label(Token::kLabel, cur_token_->second);

  ++cur_token_;
  return label;
}

Node Parser::ParseOperand()
{
  Node operand(cur_token_->first, cur_token_->second);

  ++cur_token_;
  return operand;
}

Node Parser::ParseInstruction()
{
  Node instruction(Token::kInstruction, cur_token_->second);

  ++cur_token_;
  switch (GetInstructionLength(instruction.GetString()))
  {
    case InstructionLength::kNoOperands:
    {
      break;
    }
    case InstructionLength::kTwoOperands:
    {
      instruction.SetSubNodes(TakeTwoOperands());
      break;
    }
    case InstructionLength::kThreeOperands:
    {
      instruction.SetSubNodes(TakeThreeOperands());
      break;
    }
    case InstructionLength::kTwoOperandsOptional:
    {
      instruction.SetSubNodes(TakeTwoOperandsOptional());
      break;
    }
    case InstructionLength::kThreeOperandsOptional:
    {
      instruction.SetSubNodes(TakeThreeOperandsOptional());
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
    return iter->second;
  }

  throw std::runtime_error("invalid instruction");
};

std::vector<Node> Parser::TakeTwoOperands()
{
  std::vector<Node> operands;

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

std::vector<Node> Parser::TakeThreeOperands()
{
  std::vector<Node> operands;

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

std::vector<Node> Parser::TakeTwoOperandsOptional()
{
  std::vector<Node> operands;

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

std::vector<Node> Parser::TakeThreeOperandsOptional()
{
  std::vector<Node> operands;

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
