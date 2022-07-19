#include <string>
#include <cstdint>
#include <stdexcept>

#include "src/token.h"

class Compiler
{
  public:
    uint16_t EncodeInstruction(const std::string& instruction);

  private:
    class CompilerException : std::runtime_error
    {
      public:
        CompilerException(const std::string& msg) : std::runtime_error(msg)
        {
        } 
    };

  private:
    uint16_t EncodeALUInstruction(const std::string& operation,
                                  token::TokenLine& operands);
    uint16_t EncodeBinaryALUInstruction(const std::string& operation,
                                        const std::string& Gd,
                                        const std::string& Gs,
                                        const std::string& Op2);
    uint16_t EncodeUnaryALUInstruction(const std::string& operation,
                                       const std::string& Gd,
                                       const std::string& Gs);
    uint8_t GetRegisterCode(const std::string& name);
    uint8_t GetConditionCode(const std::string& name);
    uint8_t ImmStringToInt(const std::string& Imm);
};
