cmake_minimum_required(VERSION 3.5)
project(AutomatonCompiler)

set(This AutomatonCompiler)

set(CMAKE_C_STANDARD 99)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(Headers
  src/lexer/lexer.h
  src/parser/parser.h
  src/automaton/automaton.h
)

set(Sources
  src/lexer/lexer.c
  src/parser/parser.c
  src/automaton/automaton.c
)

enable_testing()

add_subdirectory(tests)

add_library(${This}_lib STATIC ${Sources} ${Headers})
target_include_directories(${This}_lib PUBLIC src)

add_executable(${This} ${Sources} ${Headers} src/main.c)
