#pragma once
#include <string>

class TokenLine
{
  public:
    TokenLine(const std::string& line) : line_(line), iter_(line_.begin())
    {
    }

  public:
    enum class TokenType { kNone, kInstruction, kOperand, kLabel };

    struct Token { std::string str; TokenType type; };

  public:
    Token GetNextToken();

  private:
    const std::string line_;
    std::string::const_iterator iter_;
};

inline bool is_instruction(const TokenLine::Token& token)
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

inline bool is_label(const TokenLine::Token& token)
{
  return token.type == TokenLine::TokenType::kLabel;
}
