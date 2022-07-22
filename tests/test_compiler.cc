#include <gtest/gtest.h>

#include "src/compiler.h"

TEST(CompilerTests, test_compile)
{
  Compiler cmp("/home/seki/Projects/bin/program.s");
  std::string program;

  EXPECT_NO_THROW(program = cmp.Run());
  ::unlink(program.c_str());
}
