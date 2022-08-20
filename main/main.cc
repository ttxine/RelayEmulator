#include <iostream>
#include <unistd.h>

#include "core/emulator.h"
#include "compiler/run.h"
#include "utils/tempfile.h"

int main(int argc, char* argv[])
{
  std::array<uint8_t, 2> input = {};
  int input_count = 0;

  bool debug = false;
  bool is_asm = false;

  std::string help_message = "Usage: " + std::string(argv[0]) + " [OPTION] "
                             "[FILE]";

  char option;
  while ((option = getopt(argc, argv, "sdi:")) != -1)
  {
    switch (option)
    {
      case 's': is_asm = true; break;
      case 'd': debug = true; break;
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
      case '?':
      {
        std::cerr << help_message << std::endl;
        exit(EXIT_FAILURE);
      }
      default: abort();
    }
  }

  if (is_asm)
  {
    try
    {
      TemporaryFile compiled = run_compiler(
          argv[optind]);
      compiled.Close();

      Emulator emu(compiled.GetPath(), debug, input);
      emu.Run();
    }
    catch (const std::runtime_error& e)
    {
      std::cerr << argv[0] << ": error: " << e.what() << std::endl;
      std::exit(EXIT_FAILURE);
    }
  }
  else if (optind < argc)
  {
    try
    {
      Emulator emu(argv[optind], debug, input);
      emu.Run();
    }
    catch(const std::runtime_error& e)
    {
      std::cerr << argv[0] << ": error: " << e.what() << std::endl;
      exit(EXIT_FAILURE);
    }
  }
  else
  {
    std::cerr << help_message << std::endl;
    exit(EXIT_FAILURE);
  }

  return 0;
}
