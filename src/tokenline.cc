#include <iostream>

#include "src/tokenline.h"

std::string TokenLine::GetNextToken()
{
  std::string::const_iterator token_begin;

  while (std::isspace(*iter_) || *iter_ == ',') ++iter_;
  token_begin = iter_;

  while (*iter_ != '\0' && !std::isspace(*iter_) && *iter_ != ',')
    ++iter_;

  return std::string(token_begin, iter_);
}
