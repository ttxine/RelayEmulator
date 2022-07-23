#include <algorithm>

#include "src/token.h"

namespace token
{
  TokenLine::Token TokenLine::GetNextToken()
  {
    std::string::const_iterator token_begin;
    TokenType type = TokenType::kNone;

    if (iter_ == line_.begin())
    {
      type = TokenType::kInstruction;
    }

    while (std::isspace(*iter_) || *iter_ == ',')
    {
      if (*iter_ == ',')
      {
        type = TokenType::kOperand;
      }
      ++iter_;
    }
    token_begin = iter_;

    while (*iter_ != '\0' && !std::isspace(*iter_) && *iter_ != ',')
    {
      ++iter_;
    }

    std::string token(token_begin, iter_);

    while (std::isspace(*iter_))
    {
      ++iter_;
    }

    if (*iter_ == ',')
    {
      type = TokenType::kOperand;
    }

    if (token.length())
    {
      if (type == TokenType::kNone && *iter_ == '\0')
      {
        type = TokenType::kOperand;
      }
    }

    return { token, type };
  }

  std::string strtolower(const std::string& str)
  {
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    return lower;
  }
}
