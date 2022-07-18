#include <string>
#include <cstdint>
#include <stdexcept>

#include "src/tokenline.h"

class Compiler
{
  public:
    uint16_t EncodeInstruction(const std::string& instruction);
    uint8_t GetRegisterCode(const std::string& name);
    uint8_t GetConditionCode(const std::string& name);

  private:
    class CompilerException : std::runtime_error
    {
      public:
        CompilerException(const std::string& msg) : std::runtime_error(msg)
        {
        }  

      /* const char* what() const noexcept
      {
        return msg_.c_str();
      }

      private:
        std::string msg_; */
    };
};
