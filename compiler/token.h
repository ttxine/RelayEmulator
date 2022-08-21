#pragma once

enum class Token
{
  kEOF,
  kIdentifier,
  kLabel,
  kComma,
  kInstruction,
  kDirective,
  kCondition,
  kRegister,
  kNumerical,
  kComment,
  kWhiteSpace
};
