#pragma once
#include <string>
#include <cstdint>
#include <stdexcept>

#include "compiler/token.h"

class Compiler
{
  public:
    Compiler(const std::string& program_name) : to_compile_(program_name)
    {
    }

  public:
    class CompilerException : public std::runtime_error
    {
      public:
        CompilerException(const std::string& msg) : std::runtime_error(msg)
        {
        }
    };

    class InvalidInstructionException :  public CompilerException
    {
      public:
        InvalidInstructionException()
        :
        CompilerException("instruction expected")
        {
        }
    };

    class InvalidOperandException :  public CompilerException
    {
      public:
        InvalidOperandException()
        :
        CompilerException("operand expected")
        {
        }
    };

  public:
    std::string Run();

  private:
    uint16_t EncodeInstruction(const std::string& instruction);
    uint8_t GetRegisterCode(const std::string& name);
    uint8_t GetConditionCode(const std::string& name);
    uint8_t ImmStringToInt(const std::string& Imm);

  private:
    uint16_t EncodeHaltInstruction();

    uint16_t EncodeNopInstruction();

    uint16_t EncodeLoadInstruction(TokenLine::Token& G, TokenLine::Token& P);

    uint16_t EncodeStoreInstruction(TokenLine::Token& G, TokenLine::Token& P);

    uint16_t EncodeCallInstruction(TokenLine::Token& cond,
                                   TokenLine::Token& Imm);

    uint16_t EncodeJmpInstruction(TokenLine::Token& cond,
                                  TokenLine::Token& Imm);

    uint16_t EncodeMoviInstruction(
        TokenLine::Token& cond,
        TokenLine::Token& Gd, 
        TokenLine::Token& Imm);

    uint16_t EncodeMovInstruction(TokenLine::Token& Gd, TokenLine::Token& Gs);

    uint16_t EncodeAluInstruction(
        TokenLine::Token& operation,
        TokenLine::Token& Gd,
        TokenLine::Token& Gs,
        TokenLine::Token& Op2);

  private:
    uint16_t EncodeBinaryAluInstruction(
        TokenLine::Token& operation,
        TokenLine::Token& Gd,
        TokenLine::Token& Gs,
        TokenLine::Token& Op2);

    uint16_t EncodeUnaryAluInstruction(
        TokenLine::Token& operation,
        TokenLine::Token& Gd,
        TokenLine::Token& Gs);

  private:
    std::string to_compile_;
};
