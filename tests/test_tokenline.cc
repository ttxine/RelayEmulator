#include <gtest/gtest.h>

#include "src/token.h"

TEST(TokenLineTests, test_get_next_token_with_one_operand)
{
  token::TokenLine tline("  JMP 0x05");

  token::TokenLine::Token token;

  token = tline.GetNextToken();
  EXPECT_EQ(token.str, "JMP");
  EXPECT_EQ(token.type, token::TokenLine::TokenType::kInstruction);

  token = tline.GetNextToken();
  EXPECT_EQ(token.str, "0x05");
  EXPECT_EQ(token.type, token::TokenLine::TokenType::kOperand);

  EXPECT_TRUE(token::is_none(tline.GetNextToken()));
}

TEST(TokenLineTests, test_get_next_token_with_two_operands)
{
  token::TokenLine tline("  LOAD A, M");

  token::TokenLine::Token token;

  token = tline.GetNextToken();
  EXPECT_EQ(token.str, "LOAD");
  EXPECT_EQ(token.type, token::TokenLine::TokenType::kInstruction);

  token = tline.GetNextToken();
  EXPECT_EQ(token.str, "A");
  EXPECT_EQ(token.type, token::TokenLine::TokenType::kOperand);

  token = tline.GetNextToken();
  EXPECT_EQ(token.str, "M");
  EXPECT_EQ(token.type, token::TokenLine::TokenType::kOperand);

  EXPECT_TRUE(token::is_none(tline.GetNextToken()));
}

TEST(TokenLineTests, test_get_next_token_instruction_only)
{
  token::TokenLine tline("  HALT");

  token::TokenLine::Token itoken = tline.GetNextToken();
  EXPECT_EQ(itoken.str, "HALT");
  EXPECT_EQ(itoken.type, token::TokenLine::TokenType::kInstruction);

  EXPECT_TRUE(token::is_none(tline.GetNextToken()));
}
