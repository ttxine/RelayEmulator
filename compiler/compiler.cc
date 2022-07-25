#include "compiler/preprocessor.h"
#include "compiler/assembler.h"
#include "compiler/compiler.h"

std::string Compiler::Run()
{
  std::string preprocessed = Preprocess(to_compile_);
  return Assemble(preprocessed);
}

std::string Compiler::Preprocess(const std::string& to_preprocess)
{
  Preprocessor pp(to_preprocess);
  return pp.Run();
}

std::string Compiler::Assemble(const std::string& to_assemble)
{
  Assembler as(to_assemble);
  return as.Run();
}
