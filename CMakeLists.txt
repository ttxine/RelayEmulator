cmake_minimum_required(VERSION 3.22)

project(RelayEmulator LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 14) # for gtest

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
    core/memory.cc
    core/cpu.cc
    core/emulator.cc
    compiler/token.cc
    compiler/preprocessor.cc
    compiler/compiler.cc
    utils/utils.cc)

set(TEST_SOURCES
    tests/test_tokenline.cc)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(relay_emulator ${SOURCES} main/main.cc)

target_include_directories(relay_emulator PRIVATE ${PROJECT_SOURCE_DIR})

add_executable(relay_emulator_tests ${SOURCES} ${TEST_SOURCES} tests/test_main.cc)

target_include_directories(relay_emulator_tests PRIVATE ${PROJECT_SOURCE_DIR})

target_link_libraries(relay_emulator_tests PRIVATE gtest)
