#include <iostream>
#include <unistd.h>

#include "src/compiler.h"
#include "src/emulator.h"

int main(int argc, char* argv[]) {
  std::string program_path;

  uint8_t input[2];
  int input_count = 0;

  bool debug = false;

  std::string help_message = "Usage: " + std::string(argv[0])\
    + " [OPTION] [FILE] [INPUT]";

  char option;
  while ((option = ::getopt(argc, argv, "s:i:d")) != -1)
  {
    switch (option)
    {
      case 's':
      {
        try
        {
          Compiler cmp(optarg);
          program_path = cmp.Run();
        }
        catch (const std::runtime_error& e)
        {
          std::cerr << e.what() << std::endl;
          ::exit(EXIT_FAILURE);
        }
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
          ::exit(EXIT_FAILURE);
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
        ::exit(EXIT_FAILURE);
      }
      default:
      {
        ::abort();
      }
    }
  }

  if (program_path.empty())
  {
    if (optind < argc)
    {
      program_path = argv[optind];
    }
    else
    {
      {
        std::cerr << help_message << std::endl;
        ::exit(EXIT_FAILURE);
      }
    }
  }

  try
  {
    relay::Emulator emu(program_path, input, debug);
    emu.Run();
  }
  catch(const std::runtime_error& e)
  {
    std::cerr << e.what() << std::endl;
    ::exit(EXIT_FAILURE);
  }

  return 0;
}
