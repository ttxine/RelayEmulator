#include <gtest/gtest.h>

#include "src/token.h"

TEST(TokenLineTests, test_get_next_token)
{
  token::TokenLine tline("  load A, M");

  EXPECT_EQ(tline.GetNextToken().str, "load");

  EXPECT_EQ(tline.GetNextToken().str, "A");

  EXPECT_EQ(tline.GetNextToken().str, "M");

  EXPECT_TRUE(token::is_none(tline.GetNextToken()));
}
