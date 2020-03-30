cmake_minimum_required(VERSION 3.13)
project(qemu-diff)

add_library(${PROJECT_NAME} SHARED)

target_sources(${PROJECT_NAME}
  PRIVATE
  ./src/diff-test.c
  ./src/gdb-host.c
  ./src/protocol.c
  ./src/isa/x86/init.c)

target_include_directories(${PROJECT_NAME}
  PRIVATE include)

target_include_directories(${PROJECT_NAME}
  PRIVATE src/isa/x86/include)
