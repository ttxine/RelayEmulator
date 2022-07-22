#pragma once
#include <string>
#include <cstdint>
#include <stdexcept>

#include "src/token.h"

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
        CompilerException("Invalid instruction")
        {
        }
    };

    class InvalidOperandException :  public CompilerException
    {
      public:
        InvalidOperandException()
        :
        CompilerException("Invalid operand")
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

    uint16_t EncodeLoadInstruction(token::TokenLine::Token& G,
                                   token::TokenLine::Token& P);

    uint16_t EncodeStoreInstruction(token::TokenLine::Token& G,
                                    token::TokenLine::Token& P);

    uint16_t EncodeCallInstruction(token::TokenLine::Token& cond,
                                   token::TokenLine::Token& Imm);

    uint16_t EncodeJmpInstruction(token::TokenLine::Token& cond,
                                  token::TokenLine::Token& Imm);

    uint16_t EncodeMoviInstruction(
        token::TokenLine::Token& cond,
        token::TokenLine::Token& Gd, 
        token::TokenLine::Token& Imm);

    uint16_t EncodeMovInstruction(token::TokenLine::Token& Gd,
                                  token::TokenLine::Token& Gs);

    uint16_t EncodeAluInstruction(
        token::TokenLine::Token& operation,
        token::TokenLine::Token& Gd,
        token::TokenLine::Token& Gs,
        token::TokenLine::Token& Op2);

  private:
    uint16_t EncodeBinaryAluInstruction(
        token::TokenLine::Token& operation,
        token::TokenLine::Token& Gd,
        token::TokenLine::Token& Gs,
        token::TokenLine::Token& Op2);

    uint16_t EncodeUnaryAluInstruction(
        token::TokenLine::Token& operation,
        token::TokenLine::Token& Gd,
        token::TokenLine::Token& Gs);

  private:
    std::string to_compile_;
};
