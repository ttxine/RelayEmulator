#pragma once
#include <regex>
#include <string>
#include <vector>

#include "compiler/token.h"

class Lexer
{
  public:
    Lexer(const std::string& characters) : characters_(characters)
    {
    }

  public:
    std::vector<std::pair<Token, std::string>> Tokenize();

  private:
    const std::string characters_;
    std::pair<std::regex, Token> token_expressions_[41] = {
      { std::regex("^[ :\t\n]+"), Token::kWhiteSpace },
      { std::regex("^0x[0-9]+"), Token::kNumerical },
      { std::regex("^[0-9]+"), Token::kNumerical },
      { std::regex("^;[^\n]*"), Token::kComment },
      { std::regex("^[A-Za-z_]+(?=:)"), Token::kLabel },
      { std::regex("^,"), Token::kComma },
      { std::regex("^halt(?=\\W)", std::regex_constants::icase),
                   Token::kInstruction },
      { std::regex("^nop(?=\\W)", std::regex_constants::icase),
                   Token::kInstruction },
      { std::regex("^load(?=\\W)", std::regex_constants::icase),
                   Token::kInstruction },
      { std::regex("^store(?=\\W)", std::regex_constants::icase),
                   Token::kInstruction },
      { std::regex("^call(?=\\W)", std::regex_constants::icase),
                   Token::kInstruction },
      { std::regex("^jmp(?=\\W)", std::regex_constants::icase),
                   Token::kInstruction },
      { std::regex("^movi(?=\\W)", std::regex_constants::icase),
                   Token::kInstruction },
      { std::regex("^mov(?=\\W)", std::regex_constants::icase),
                   Token::kInstruction },
      { std::regex("^adc(?=\\W)", std::regex_constants::icase),
                   Token::kInstruction },
      { std::regex("^add(?=\\W)", std::regex_constants::icase),
                   Token::kInstruction },
      { std::regex("^sbc(?=\\W)", std::regex_constants::icase),
                   Token::kInstruction },
      { std::regex("^sub(?=\\W)", std::regex_constants::icase),
                   Token::kInstruction },
      { std::regex("^and(?=\\W)", std::regex_constants::icase),
                   Token::kInstruction },
      { std::regex("^or(?=\\W)", std::regex_constants::icase),
                   Token::kInstruction },
      { std::regex("^xor(?=\\W)", std::regex_constants::icase),
                   Token::kInstruction },
      { std::regex("^not(?=\\W)", std::regex_constants::icase),
                   Token::kInstruction },
      { std::regex("^ror(?=\\W)", std::regex_constants::icase),
                   Token::kInstruction },
      { std::regex("^shr(?=\\W)", std::regex_constants::icase),
                   Token::kInstruction },
      { std::regex("^rcr(?=\\W)", std::regex_constants::icase),
                   Token::kInstruction },
      { std::regex("^f(?=\\W)", std::regex_constants::icase),
                   Token::kRegister },
      { std::regex("^a(?=\\W)", std::regex_constants::icase),
                   Token::kRegister },
      { std::regex("^b(?=\\W)", std::regex_constants::icase),
                   Token::kRegister },
      { std::regex("^c(?=\\W)", std::regex_constants::icase),
                   Token::kRegister },
      { std::regex("^d(?=\\W)", std::regex_constants::icase),
                   Token::kRegister },
      { std::regex("^m(?=\\W)", std::regex_constants::icase),
                   Token::kRegister },
      { std::regex("^s(?=\\W)", std::regex_constants::icase),
                   Token::kRegister },
      { std::regex("^l(?=\\W)", std::regex_constants::icase),
                   Token::kRegister },
      { std::regex("^pc(?=\\W)", std::regex_constants::icase),
                   Token::kRegister },
      { std::regex("^z(?=\\W)", std::regex_constants::icase),
                   Token::kCondition },
      { std::regex("^ns(?=\\W)", std::regex_constants::icase),
                   Token::kCondition },
      { std::regex("^c(?=\\W)", std::regex_constants::icase),
                   Token::kCondition },
      { std::regex("^nc(?=\\W)", std::regex_constants::icase),
                   Token::kCondition },
      { std::regex("^s(?=\\W)", std::regex_constants::icase),
                   Token::kCondition },
      { std::regex("^nz(?=\\W)", std::regex_constants::icase),
                   Token::kCondition },
      { std::regex("^[A-Za-z][A-Za-z0-9_]*"), Token::kIdentifier }
    };
};
