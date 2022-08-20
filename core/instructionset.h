#pragma once
#include <cstdint>

// Instruction codes without conditions and operands to determine each
// instruction.
enum InstructionCode : uint16_t
{
  kHALT       = 0x1000,
  kNOP        = 0x0000,
  kLOAD       = 0x2800,
  kLOADI      = 0x2000,
  kSTORE      = 0x3800,
  kSTOREI     = 0x3000,
  kCALL       = 0x8F00,
  kJMP        = 0x8700,
  kMOVI       = 0x8000,
  kMOV        = 0x1800,
  kALU        = 0x4000
};

inline bool is_HALT(uint16_t instruction)
{
  return instruction == kHALT;
}

inline bool is_NOP(uint16_t instruction)
{
  return instruction == kNOP;
}

inline bool is_LOAD(uint16_t instruction)
{
  return (instruction & 0xF800) == kLOAD;
}

inline bool is_LOADI(uint16_t instruction)
{
  return (instruction & 0xF800) == kLOADI;
}

inline bool is_STORE(uint16_t instruction)
{
  return (instruction & 0xF800) == kSTORE;
}

inline bool is_STOREI(uint16_t instruction)
{
  return (instruction & 0xF800) == kSTOREI;
}

inline bool is_CALL(uint16_t instruction)
{
  return (instruction & 0x8F00) == kCALL;
}

inline bool is_JMP(uint16_t instruction)
{
  return (instruction & 0x8700) == kJMP;
}

inline bool is_MOVI(uint16_t instruction)
{
  return (instruction & 0x8000) == kMOVI;
}

inline bool is_MOV(uint16_t instruction)
{
  return (instruction & 0x1800) == kMOV;
}

inline bool is_ALU(uint16_t instruction)
{
  return (instruction & 0xC000) == kALU;
}
