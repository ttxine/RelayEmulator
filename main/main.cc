#include <iostream>
#include <unistd.h>

#include "core/emulator.h"
#include "compiler/run.h"
#include "utils/tempfile.h"

void clear(const std::string& prog, const std::string& executable_path,
           bool is_asm)
{
  if (is_asm)
  {
    try
    {
      unlink_temporary_file(executable_path);
    }
    catch (const std::runtime_error& e)
    {
      std::cerr << prog << "error: " << e.what() << std::endl;
      exit(EXIT_FAILURE);
    }
  }
}

int main(int argc, char* argv[])
{
  std::array<uint8_t, 2> input = {};
  int input_count = 0;

  bool debug = false;
  bool is_asm = false;

  std::string help_message = "Usage: " + std::string(argv[0])\
    + " [OPTION] [FILE]";

  char option;
  while ((option = getopt(argc, argv, "si:d")) != -1)
  {
    switch (option)
    {
      case 's':
      {
        is_asm = true;
        break;
      }
      case 'i':
      {
        if (input_count < 2)
        {
          input[input_count] = std::stoi(optarg);
          ++input_count;
        }
        else
        {
          std::cerr << help_message << std::endl;
          exit(EXIT_FAILURE);
        }
        break;
      }
      case 'd':
      {
        debug = true;
        break;
      }
      case '?':
      {
        std::cerr << help_message << std::endl;
        exit(EXIT_FAILURE);
      }
      default:
      {
        abort();
      }
    }
  }

  std::string executable_path;
  if (is_asm)
  {
    try
    {
      executable_path = run_compiler(argv[optind]);
    }
    catch (const std::runtime_error& e)
    {
      std::cerr << argv[0] << ": " << e.what() << std::endl;
      std::exit(EXIT_FAILURE);
    }
  }
  else if (optind < argc)
  {
    executable_path = argv[optind];
  }
  else
  {
    std::cerr << help_message << std::endl;
    exit(EXIT_FAILURE);
  }

  try
  {
    Emulator emu(executable_path, debug, input);
    emu.Run();
  }
  catch(const std::runtime_error& e)
  {
    std::cerr << argv[0] << ": error: emulator: " << e.what() << std::endl;

    clear(argv[0], executable_path, is_asm);
    exit(EXIT_FAILURE);
  }

  clear(argv[0], executable_path, is_asm);
  return 0;
}
