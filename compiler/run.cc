#include <fstream>
#include <iostream>

#include "compiler/run.h"

static std::string get_characters_from_stream(std::ifstream& stream);

TemporaryFile run_compiler(const std::string& path)
{
  std::ifstream in(path, std::ifstream::ate | std::ios::binary);
  std::string characters = get_characters_from_stream(in);

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

static std::string get_characters_from_stream(std::ifstream& stream)
{
  std::streamsize bytes = stream.tellg();

  if (stream.fail()) throw std::runtime_error("can't open a file");

  std::string characters;
  characters.resize(bytes);

  stream.seekg(0, std::ios::beg);
  stream.read(&characters[0], bytes);

  return characters;
}
