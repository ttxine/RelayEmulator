cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(RelayEmulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
    core/rom.cc
    core/cpu.cc
    core/bus.cc
    core/disassembler.cc
    core/emulator.cc
    compiler/lexer.cc
    compiler/parser.cc
    compiler/compiler.cc
    compiler/run.cc
    utils/str.cc
    utils/tempfile.cc)

set(UI_SOURCES
    ui/statebox.cc
    ui/staterow.cc
    ui/inputdialog.cc
    ui/mainform.cc)

include_directories(${PROJECT_SOURCE_DIR})

find_package(wxWidgets COMPONENTS core base)
if(wxWidgets_FOUND)
    include(${wxWidgets_USE_FILE})
    add_executable(relay-emulator-gui ${SOURCES} ${UI_SOURCES} ui/app.cc)
    target_link_libraries(relay-emulator-gui PRIVATE ${wxWidgets_LIBRARIES})
else(wxWidgets_FOUND)
    message(WARNING "wxWidgets not found. GUI version of emulator will not be built.")
endif(wxWidgets_FOUND)

add_executable(relay-emulator ${SOURCES} main/main.cc)
