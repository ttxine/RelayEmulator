#include <fstream>
#include <iostream>

#include "compiler/run.h"

static std::string file_to_string(const std::string& path);

TemporaryFile run_compiler(const std::string& path)
{
  std::string characters = file_to_string(path);

  std::vector<std::pair<Token, std::string>> tokens;
  try
  {
    Lexer lexer(characters);
    tokens = lexer.Tokenize();
  }
  catch (const std::runtime_error& e)
  {
    throw std::runtime_error("lexer: " + std::string(e.what()));
  }

  std::vector<Node> root;
  std::unordered_map<std::string, int> labels;
  try
  {
    Parser parser(tokens);
    root = parser.Parse();
    labels = parser.GetLabels();
  }
  catch (const std::runtime_error& e)
  {
    throw std::runtime_error("parser: " + std::string(e.what()));
  }

  try
  {
    Compiler compiler(root, labels);
    return compiler.Compile();
  }
  catch (const std::runtime_error& e)
  {
    throw std::runtime_error("compiler: " + std::string(e.what()));
  }
}

static std::string file_to_string(const std::string& path)
{
  std::ifstream file(path, std::ifstream::ate | std::ios::binary);

  if (file.fail())
  {
    throw std::runtime_error("can't open a file: \"" + path + "\"");
  }

  std::streamsize bytes = file.tellg();

  std::string characters;
  characters.resize(bytes);

  file.seekg(0, std::ios::beg);
  file.read(&characters[0], bytes);

  return characters;
}
