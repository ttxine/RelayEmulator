#include <stdexcept>

#include "compiler/lexer.h"

std::vector<std::pair<Token, std::string>> Lexer::Tokenize()
{
  std::vector<std::pair<Token, std::string>> tokens;
  std::string::const_iterator token_begin = characters_.begin();

  while (token_begin < characters_.end())
  {
    std::match_results<std::string::const_iterator> match;

    for (std::pair<std::regex, Token> token_expression : token_expressions_)
    {
      std::regex pattern = token_expression.first;

      if (std::regex_search(token_begin, characters_.end(), match, pattern))
      {
        if (token_expression.second != Token::kWhiteSpace
            && token_expression.second != Token::kComment)
        {
          tokens.push_back({ token_expression.second, match[0].str() });
        }

        token_begin = match[0].second;
        break;
      }
    }

    if (match.empty())
    {
      throw std::runtime_error("unknown token");
    }
  }

  return tokens;
}
