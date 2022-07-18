#include "src/emulator.h"

int main(int argc, char* argv[]) {
  if (argc > 1)
  {
    if (argc > 2)
    {
      uint8_t input[2];
      input[0] = static_cast<uint8_t>(atoi(argv[2]));

      if (argc > 3)
        input[1] = static_cast<uint8_t>(atoi(argv[3]));

      relay::Emulator emu = relay::Emulator(argv[1], input, true);
      emu.Run();
    }
    else
    {
      relay::Emulator emu = relay::Emulator(argv[1], true);
      emu.Run();
    }
  }
  else
    throw std::exception();
}
