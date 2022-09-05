#include <iostream>
#include <unistd.h>

#include "main/main.h"
#include "core/emulator.h"
#include "compiler/run.h"

int main(int argc, char* argv[])
{
  Options options = parse_options(argc, argv);

  if (options.is_asm)
  {
    try
    {
      TemporaryFile compiled = run_compiler(argv[optind]);
      compiled.Close();

      Emulator emu(compiled.GetPath(), options.input);
      options.debug ? emu.Debug() : emu.Run();
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
      Emulator emu(argv[optind], options.input);
      options.debug ? emu.Debug() : emu.Run();
    }
    catch(const std::runtime_error& e)
    {
      std::cerr << argv[0] << ": error: " << e.what() << std::endl;
      exit(EXIT_FAILURE);
    }
  }
  else
  {
    print_help(argv[0]);
    exit(EXIT_FAILURE);
  }

  return 0;
}

Options parse_options(int argc, char* argv[])
{
  Options options;
  int input_count = 0;

  char option;
  while ((option = getopt(argc, argv, "hsdi:")) != -1)
  {
    switch (option)
    {
      case 's':
      {
        options.is_asm = true;
        break;
      }
      case 'd':
      {
        options.debug = true;
        break;
      }
      case 'i':
      {
        if (input_count < 2)
        {
          options.input[input_count] = std::stoi(optarg);
          ++input_count;
        }
        else
        {
          print_help(argv[0]);
          exit(EXIT_FAILURE);
        }
        break;
      }
      case 'h': case '?': default:
      {
        print_help(argv[0]);
        exit(EXIT_FAILURE);
      }
    }
  }

  return options;
}

void print_help(const std::string& binary)
{
  std::cerr << "RelayEmulator - https://github.com/ttxine/RelayEmulator\n"
               "\n"
               "Usage: " << binary << " [options] [path to file]\n"
               "\n"
               "Options:\n"
               "  -h                            Display this help message.\n"
               "  -s                            Compile file before execution.\n"
               "  -i <value>                    Add value to input (can be used twice).\n"
               "  -d                            Debug mode.\n" <<
               std::endl;
}
