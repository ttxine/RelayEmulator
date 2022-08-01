#pragma once
#include <string>
#include <vector>
#include <memory>

#include "compiler/token.h"

template <class T>
class BaseNode
{
  public:
    BaseNode()
    {
    }
    BaseNode(const BaseNode&) = delete;
    BaseNode& operator=(const BaseNode&) = delete;
    BaseNode(BaseNode&& other) = default;
    BaseNode& operator=(BaseNode&& other) = default;

  public:
    const std::vector<std::unique_ptr<T>>& GetSubNodes() const
    {
      return sub_nodes_;
    };

    void AddSubNodes(std::unique_ptr<T> node)
    {
      sub_nodes_.push_back(std::move(node));
    };

    void SetSubNodes(std::vector<std::unique_ptr<T>> sub_nodes)
    {
      sub_nodes_ = std::move(sub_nodes);
    };

  protected:
    std::vector<std::unique_ptr<T>> sub_nodes_;
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
