#pragma once
#include <string>
#include <vector>

#include "compiler/token.h"

class Node
{
  public:
    Node(Token token_type, const std::string& str)
        : token_type_(token_type), str_(str)
    {
    }

  public:
    const std::vector<Node>& GetSubNodes() const { return sub_nodes_; }
    void AddSubNodes(Node node) { sub_nodes_.push_back(node); }
    void SetSubNodes(std::vector<Node> sub_nodes) { sub_nodes_ = sub_nodes; }

    const std::string& GetString() const { return str_; }
    Token GetTokenType() const { return token_type_; };

  private:
    std::string str_;
    Token token_type_;

    std::vector<Node> sub_nodes_;
};

using Root = std::vector<Node>;
