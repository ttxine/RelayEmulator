cmake_minimum_required(VERSION 3.22)

project(RelayEmulator LANGUAGES CXX)

find_package(wxWidgets REQUIRED COMPONENTS net core base)
include(${wxWidgets_USE_FILE})

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
    core/rom.cc
    core/cpu.cc
    core/bus.cc
    core/emulator.cc
    compiler/lexer.cc
    compiler/parser.cc
    compiler/compiler.cc
    utils/str.cc
    utils/tempfile.cc)

set(UI_SOURCES
    ui/panels.cc
    ui/statebox.cc
    ui/inputdialog.cc
    ui/mainform.cc)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(relay_emulator ${SOURCES} main/main.cc)
add_executable(relay_emulator_ui
               ${SOURCES}
               ${UI_SOURCES}
               ui/app.cc)

target_include_directories(relay_emulator PRIVATE ${PROJECT_SOURCE_DIR})

target_include_directories(relay_emulator_ui PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(relay_emulator_ui ${wxWidgets_LIBRARIES})
