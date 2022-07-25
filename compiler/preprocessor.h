#include <string>
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
    void InitialPreprocess(std::ifstream& in, std::fstream& out);
    void HandleDirectives(std::fstream& in);
    void PerformSubstitutions(std::fstream& in, std::ofstream& out);

  private:
    std::string to_preprocess_;
    int addr_ = 0;
    std::unordered_map<std::string, std::string> labels_;
};
