#include <gtest/gtest.h>

#include "src/tokenline.h"

TEST(TokenLineTests, test_get_next_token)
{
  TokenLine tline("  load A, M");

  EXPECT_EQ(tline.GetNextToken(), "load");
  EXPECT_EQ(tline.GetNextToken(), "A");
  EXPECT_EQ(tline.GetNextToken(), "M");
  EXPECT_TRUE(tline.GetNextToken().empty());
}
