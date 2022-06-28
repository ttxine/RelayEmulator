#include "emulator.h"

int main() {
  relay::Emulator emu = relay::Emulator(true);
  emu.Run();
}
