#include <algorithm>

#include "compiler/token.h"

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

  while (*iter_ != '\0' && !std::isspace(*iter_) && *iter_ != ','
         && *iter_ != ':')
  {
    ++iter_;
  }

  std::string token(token_begin, iter_);

  while (std::isspace(*iter_))
  {
    ++iter_;
  }

  if (*iter_ == ':')
  {
    type = TokenType::kLabel;
    ++iter_;
  }
  else if (token.length() && type == TokenType::kNone)
  {
    if (*iter_ == ',')
    {
      type = TokenType::kOperand;
    }
    else if (*iter_ == '\0')
    {
      type = TokenType::kOperand;
    }
  }

  return { token, type };
}
