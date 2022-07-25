#include <algorithm>

#include "compiler/token.h"

TokenLine::Token TokenLine::GetNextToken()
{
  TokenType type = TokenType::kNone;

  if (last_token_end_ == line_.begin())
  {
    type = TokenType::kInstruction;
  }

  while (std::isspace(*last_token_end_) || *last_token_end_ == ',')
  {
    if (*last_token_end_ == ',')
    {
      type = TokenType::kOperand;
    }
    ++last_token_end_;
  }
  last_token_begin_ = last_token_end_;

  if (*last_token_end_ == ';')
  {
    last_token_end_ = line_.end();
    return { std::string(last_token_begin_, last_token_end_),
             TokenType::kComment };
  }

  while (*last_token_end_ != '\0' && !std::isspace(*last_token_end_)
         && *last_token_end_ != ',' && *last_token_end_ != ':')
  {
    ++last_token_end_;
  }

  std::string token(last_token_begin_, last_token_end_);

  while (std::isspace(*last_token_end_))
  {
    ++last_token_end_;
  }

  if (*last_token_end_ == ':')
  {
    type = TokenType::kLabel;
    ++last_token_end_;
  }
  else if (token.length() && type == TokenType::kNone)
  {
    if (*last_token_end_ == ',')
    {
      type = TokenType::kOperand;
    }
    else if (*last_token_end_ == '\0')
    {
      type = TokenType::kOperand;
    }
  }
  else if (token.empty() && type == TokenType::kInstruction)
  {
    type = TokenType::kNone;
  }

  return { token, type };
}
