#include <stdexcept>

#include "compiler/lexer.h"

std::vector<std::pair<Token, std::string>> Lexer::Tokenize()
{
  std::vector<std::pair<Token, std::string>> tokens;
  std::string::const_iterator token_begin = characters_.begin();

  while (token_begin < characters_.end())
  {
    std::match_results<std::string::const_iterator> match;

    auto token_expr = token_expressions_.begin();
    while (token_expr < token_expressions_.end() &&
           !std::regex_search(token_begin, characters_.end(), match,
                              token_expr->first)) ++token_expr;

    if (match.empty())
    {
      throw std::runtime_error("unknown token");
    }
    else if (token_expr != token_expressions_.end() &&
             token_expr->second != Token::kWhiteSpace &&
             token_expr->second != Token::kComment)
    {
      tokens.push_back({ token_expr->second, match[0].str() });
    }

    token_begin = match[0].second;
  }

  return tokens;
}
