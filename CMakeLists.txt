cmake_minimum_required(VERSION 3.22)

project(RelayEmulator LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 11)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
    core/memory.cc
    core/cpu.cc
    core/emulator.cc
    compiler/lexer.cc
    compiler/parser.cc
    compiler/compiler.cc
    utils/str.cc
    utils/tempfile.cc)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(relay_emulator ${SOURCES} main/main.cc)

target_include_directories(relay_emulator PRIVATE ${PROJECT_SOURCE_DIR})
