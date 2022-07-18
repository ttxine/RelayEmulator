#include <string>

class TokenLine
{
  public:
    TokenLine(const std::string& line) : line_(line), iter_(line.begin())
    {
    }

  public:
    std::string GetNextToken();

  private:
    std::string line_;
    std::string::const_iterator iter_;
};
