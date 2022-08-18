#include "compiler/lexer.h"
#include "compiler/parser.h"
#include "compiler/compiler.h"

std::unique_ptr<TemporaryFile> run_compiler(const std::string& path)
{
  std::ifstream in(path, std::ifstream::ate | std::ios::binary);
  std::streamsize bytes = in.tellg();

  if (in.fail())
  {
    throw std::runtime_error("can't open a file");
  }

  std::string characters;
  characters.resize(bytes);

  in.seekg(0, std::ios::beg);
  in.read(&characters[0], bytes);

  std::vector<std::pair<Token, std::string>> tokens;
  try
  {
    Lexer lexer(characters);
    tokens = lexer.Tokenize();
  }
  catch (const std::runtime_error& e)
  {
    std::cerr << "lexer: " << e.what() << std::endl;
    std::exit(EXIT_FAILURE);
  }

  Root root;
  std::unordered_map<std::string, int> labels;
  try
  {
    Parser parser(tokens);
    root = parser.Parse();
    labels = parser.GetLabels();
  }
  catch (const std::runtime_error& e)
  {
    std::cerr << "parser: " << e.what() << std::endl;
    std::exit(EXIT_FAILURE);
  }

  std::unique_ptr<TemporaryFile> out;
  try
  {
    Compiler compiler(root, labels);
    out = compiler.Compile();
  }
  catch (const std::runtime_error& e)
  {
    std::cerr << "compiler: " << e.what() << std::endl;
    std::exit(EXIT_FAILURE);
  }

  return out;
}
