#include <regex>
#include <fstream>
#include <stdexcept>

#include "utils/utils.h"
#include "compiler/token.h"
#include "compiler/preprocessor.h"

std::string Preprocessor::Run()
{
  std::ifstream in(to_preprocess_);

  if (in.fail())
  {
    throw std::runtime_error("can't open a file: \"" + to_preprocess_ + '"');
  }

  CollectLabels(in);

  std::string path = create_temporary_file();
  std::ofstream out(path);

  std::string line;
  while (std::getline(in, line))
  {
    std::string handled_line = HandleLine(line);

    if (!handled_line.empty())
    {
      out << handled_line << std::endl;
    }
  }

  return path;
}

void Preprocessor::CollectLabels(std::ifstream& in)
{
  std::string line;

  while (std::getline(in, line))
  {
    TokenLine tline(line);

    TokenLine::Token token = tline.GetNextToken();
    if (is_label(token))
    {
      labels_[token.str] = std::to_string(addr_);
    }
    else if (!is_none(token))
    {
      ++addr_;
    }
  }

  in.clear();
  in.seekg(0);
}

std::string Preprocessor::HandleLine(const std::string& line)
{
  TokenLine tline(line);
  TokenLine::Token token = tline.GetNextToken();

  if (is_none(token) || is_label(token))
  {
    return "";
  }
  else if (is_instruction(token))
  {
    ++addr_;

    while (is_operand(token = tline.GetNextToken()))
    {
      if (labels_.find(token.str) != labels_.end())
      {
        return std::regex_replace(line, std::regex(token.str),
                                  labels_[token.str]);
      }
    }

    return line;
  }
  else
  {
    throw std::runtime_error("instruction or label expected");
  }
}
