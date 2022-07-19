#include <gtest/gtest.h>

#include "src/compiler.h"

TEST(CompilerTests, test_encode_instruction)
{
  Compiler cmp;

  ASSERT_EQ(cmp.EncodeInstruction("load A, 0x80"), 0x2080);
  ASSERT_EQ(cmp.EncodeInstruction("load B, S"), 0x2905);
};

TEST(CompilerTests, test_encode_instruction_with_optional_operand)
{
  Compiler cmp;

  ASSERT_EQ(cmp.EncodeInstruction("call 0x55"), 0x8F55);
  ASSERT_EQ(cmp.EncodeInstruction("call NS, 0x17"), 0xAF17);
}

TEST(CompilerTests, test_encode_binary_alu_instruction)
{
  Compiler cmp;

  ASSERT_EQ(cmp.EncodeInstruction("add A, A, B"), 0x4809);
}

TEST(CompilerTests, test_encode_unary_alu_instruction)
{
  Compiler cmp;

  ASSERT_EQ(cmp.EncodeInstruction("shr A, A"), 0x780C);
}
