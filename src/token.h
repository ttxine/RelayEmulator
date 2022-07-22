#pragma once
#include <string>

namespace token
{
  class TokenLine
  {
    public:
      TokenLine(const std::string& line) : line_(line), iter_(line.begin())
      {
      }

    public:
      enum class TokenType { kNone, kInstruction, kOperand };

      struct Token { std::string str; TokenType type; };

    public:
      Token GetNextToken();

    private:
      std::string line_;
      std::string::const_iterator iter_;
  };

  std::string strtolower(const std::string& str);

  inline bool is_intruction(const TokenLine::Token& token)
  {
    return token.type == TokenLine::TokenType::kInstruction;
  }

  inline bool is_operand(const TokenLine::Token& token)
  {
    return token.type == TokenLine::TokenType::kOperand;
  }

  inline bool is_none(const TokenLine::Token& token)
  {
    return token.type == TokenLine::TokenType::kNone;
  }
}
