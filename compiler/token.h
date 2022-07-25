#pragma once
#include <string>

class TokenLine
{
  public:
    enum class TokenType { kNone, kInstruction, kOperand, kLabel, kComment };

    struct Token { std::string str; TokenType type; };

  public:
    TokenLine(const std::string& line)
        : line_(line),
          last_token_begin_(line_.begin()),
          last_token_end_(line_.begin())
    {
    }

  public:
    Token GetNextToken();

    std::string GetLine() const
    {
      return line_;
    }

    void ReplaceLastToken(const std::string& str)
    {
      line_.replace(last_token_begin_, last_token_end_, str);
      last_token_end_ = last_token_begin_ + str.length();
    }

  private:
    std::string line_;
    std::string::const_iterator last_token_begin_;
    std::string::const_iterator last_token_end_;
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

inline bool is_comment(const TokenLine::Token& token)
{
  return token.type == TokenLine::TokenType::kComment;
}
