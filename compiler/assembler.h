#pragma once
#include <string>
#include <cstdint>
#include <stdexcept>

#include "compiler/token.h"

class Assembler
{
  public:
    Assembler(const std::string& program_name) : to_assemble_(program_name)
    {
    }

  public:
    class AssemblerException : public std::runtime_error
    {
      public:
        AssemblerException(const std::string& msg) : std::runtime_error(msg)
        {
        }
    };

    class InvalidInstructionException :  public AssemblerException
    {
      public:
        InvalidInstructionException()
        :
        AssemblerException("instruction expected")
        {
        }
    };

    class InvalidOperandException :  public AssemblerException
    {
      public:
        InvalidOperandException()
        :
        AssemblerException("operand expected")
        {
        }
    };

  public:
    std::string Run();

  private:
    uint16_t EncodeInstruction(const std::string& instruction);
    uint8_t GetRegisterCode(const std::string& name);
    uint8_t GetConditionCode(const std::string& name);

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
    std::string to_assemble_;
};
