#include <algorithm>

#include "src/token.h"

namespace token
{
  TokenLine::Token TokenLine::GetNextToken()
  {
    std::string::const_iterator token_begin;
    TokenType type = TokenType::kNone;

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

    if (token.length())
    {
      while (std::isspace(*iter_))
      {
        ++iter_;
      }

      if (*iter_ == ',')
      {
        type = TokenType::kOperand;
      }
      else if (type == TokenType::kNone)
      {
        type = TokenType::kInstruction;
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
