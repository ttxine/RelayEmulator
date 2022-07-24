#include <string>
#include <iostream>
#include <unordered_map>

class Preprocessor
{
  public:
    Preprocessor(const std::string& program_name)
    : to_preprocess_(program_name)
    {
    }

  public:
    std::string Run();
    void CollectLabels(std::ifstream& in);
    std::string HandleLine(const std::string& line);

  private:
    std::string to_preprocess_;
    int addr_ = 0;
    std::unordered_map<std::string, std::string> labels_;
};
