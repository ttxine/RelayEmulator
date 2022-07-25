#include <string>

class Compiler
{
  public:
    Compiler(const std::string& program_name) : to_compile_(program_name)
    {
    }

  public:
    std::string Run();

  private:
    std::string Preprocess(const std::string& to_preprocess);
    std::string Assemble(const std::string& to_assemble);

  private:
    std::string to_compile_;
};
