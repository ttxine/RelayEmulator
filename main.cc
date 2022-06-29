#include "src/emulator.h"

int main() {
  relay::Emulator emu = relay::Emulator(false);
  emu.Run();
}
