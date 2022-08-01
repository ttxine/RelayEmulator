#pragma once
#include <string>
#include <vector>

#include "compiler/token.h"

template <class T>
class BaseNode
{
  public:
    BaseNode()
    {
    }

  public:
    const std::vector<T>& GetSubNodes() const
    {
      return sub_nodes_;
    };

    void AddSubNodes(T node)
    {
      sub_nodes_.push_back(node);
    };

    void SetSubNodes(std::vector<T> sub_nodes)
    {
      sub_nodes_ = sub_nodes;
    };

  protected:
    std::vector<T> sub_nodes_;
};

class Node : public BaseNode<Node>
{
  public:
    Node(Token token_type, const std::string& str)
        : token_type_(token_type), str_(str)
    {
    }

  public:
    const std::string& GetString() const { return str_; }
    Token GetTokenType() const { return token_type_; };

  private:
    std::string str_;
    Token token_type_;
};

class Root : public BaseNode<Node>
{
};
