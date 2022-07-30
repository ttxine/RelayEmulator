#pragma once

enum class Token
{
  kIdentifier,
  kLabel,
  kComma,
  kInstruction,
  kCondition,
  kRegister,
  kNumerical,
  kComment,
  kWhiteSpace
};
