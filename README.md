# Relay Emulator
**Relay Emulator is a [relay computer](https://github.com/Dovgalyuk/Relay)
emulator**, made in C++. It provides command-line tools for executing and
debugging programs and a GUI for user-friendly input, program load and
displaying computer state.

<p align="center">
    <img
     style="border-radius: 0.25rem"
     src="https://user-images.githubusercontent.com/89527465/186873693-bb32df63-af32-450e-9511-22e1bd08fe15.png"
     alt="GUI screenshot">
</p>

## Programs
Programs can be executed as binary or assembler files. Assembler files
must be compiled before execution, so emulator also provides compiler.

Program samples can be found on
[computer project page](https://dovgalyuk.github.io/Relay/programs.html).

## Debug
The emulator provides step-by-step program execution for debugging.

## Getting Started
### Install from sources
- Install dependencies:
    ```
    sudo apt -y install build-essential cmake
    sudo apt -y install libwxgtk3.0-gtk3-dev
    ```
- Compile sources:
    ```
    cmake -B build
    cd build
    make relay-emulator
    make relay-emulator-gui
    ```
### Binaries
- `relay-emulator` binary is command-line emulator
- `relay-emulator-gui` binary is emulator with GUI
