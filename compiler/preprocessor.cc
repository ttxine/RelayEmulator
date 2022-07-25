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

  std::string init_path = create_temporary_file();
  std::fstream init(init_path);

  InitialPreprocess(in, init);

  init.clear();
  init.seekp(0);
  in.close();

  HandleDirectives(init);

  init.clear();
  init.seekg(0);

  std::string out_path = create_temporary_file();
  std::ofstream out(out_path);

  PerformSubstitutions(init, out);

  unlink_temporary_file(init_path);

  return out_path;
}

void Preprocessor::InitialPreprocess(std::ifstream& in, std::fstream& out)
{
  std::string line;
  while (std::getline(in, line))
  {
    TokenLine tline(line);

    TokenLine::Token token;
    while (!is_none(token = tline.GetNextToken()))
    {
      if (is_comment(token))
      {
        tline.ReplaceLastToken("");
      }
    }

    if (!line.empty())
    {
      out << tline.GetLine() << std::endl;
    }
  }
}

void Preprocessor::HandleDirectives(std::fstream& in)
{
  std::string line;
  while (std::getline(in, line))
  {
    TokenLine tline(line);
    TokenLine::Token token = tline.GetNextToken();
    if (is_instruction(token))
    {
      ++addr_;
    }
    else if (is_label(token))
    {
      labels_[token.str] = std::to_string(addr_);
    }
    else if (strtolower(line) == "org")
    {
      token = tline.GetNextToken();

      if (is_operand(token))
      {
        addr_ = asm_stoi(token.str);
      }
      else
      {
        throw std::runtime_error("operand expected");
      }
    }
    else
    {
      throw std::runtime_error("instruction or directive expected");
    }
  }
}

void Preprocessor::PerformSubstitutions(std::fstream& in, std::ofstream& out)
{
  std::string line;
  while (std::getline(in, line))
  {
    TokenLine tline(line);
    TokenLine::Token token = tline.GetNextToken();

    std::string handled_line = line;

    if (is_none(token) || is_label(token))
    {
      handled_line = "";
    }
    else if (is_instruction(token))
    {
      ++addr_;

      while (!is_none(token = tline.GetNextToken()))
      {
        if (is_operand(token))
        {
          auto label = labels_.find(token.str);
          if (label != labels_.end())
          {
            tline.ReplaceLastToken(label->second);
            handled_line = tline.GetLine();
          }
        }
        else
        {
          throw std::runtime_error("operand expected");
        }
      }
    }
    else
    {
      throw std::runtime_error("instruction or label expected");
    }

    if (!handled_line.empty())
    {
      out << handled_line << std::endl;
    }
  }
}
