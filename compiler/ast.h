#pragma once
#include <string>
#include <vector>
#include <memory>

#include "compiler/token.h"

struct Node
{
  std::string str;

  virtual ~Node() = default;
};

struct LabelNode : public Node
{
};

struct OperandNode : public Node
{
  Token type;
};

struct InstructionNode : public Node
{
  std::vector<std::unique_ptr<OperandNode>> operands;
};

struct Root
{
  std::vector<std::unique_ptr<Node>> nodes;
};
