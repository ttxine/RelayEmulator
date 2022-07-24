#include <gtest/gtest.h>

#include "compiler/token.h"

TEST(TokenLineTests, test_get_next_token_with_one_operand)
{
  TokenLine tline("  JMP 0x05");

  TokenLine::Token token;

  token = tline.GetNextToken();
  EXPECT_EQ(token.str, "JMP");
  EXPECT_EQ(token.type, TokenLine::TokenType::kInstruction);

  token = tline.GetNextToken();
  EXPECT_EQ(token.str, "0x05");
  EXPECT_EQ(token.type, TokenLine::TokenType::kOperand);

  EXPECT_TRUE(is_none(tline.GetNextToken()));
}

TEST(TokenLineTests, test_get_next_token_instruction_with_two_operands)
{
  TokenLine tline("  LOAD A, , M");

  TokenLine::Token token;

  token = tline.GetNextToken();
  EXPECT_EQ(token.str, "LOAD");
  EXPECT_EQ(token.type, TokenLine::TokenType::kInstruction);

  token = tline.GetNextToken();
  EXPECT_EQ(token.str, "A");
  EXPECT_EQ(token.type, TokenLine::TokenType::kOperand);

  token = tline.GetNextToken();
  EXPECT_EQ(token.str, "M");
  EXPECT_EQ(token.type, TokenLine::TokenType::kOperand);

  EXPECT_TRUE(is_none(tline.GetNextToken()));
}

TEST(TokenLineTests, test_get_next_token_instruction_only)
{
  TokenLine tline("  HALT");

  TokenLine::Token itoken = tline.GetNextToken();
  EXPECT_EQ(itoken.str, "HALT");
  EXPECT_EQ(itoken.type, TokenLine::TokenType::kInstruction);

  EXPECT_TRUE(is_none(tline.GetNextToken()));
}

TEST(TokenLineTests, test_get_next_token_label)
{
  TokenLine tline("Label:");
  
  TokenLine::Token itoken = tline.GetNextToken();
  EXPECT_EQ(itoken.str, "Label");
  EXPECT_EQ(itoken.type, TokenLine::TokenType::kLabel);

  EXPECT_TRUE(is_none(tline.GetNextToken()));
}
