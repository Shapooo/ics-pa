cmake_minimum_required(VERSION 3.8)

project(nano-lite)

add_executable(nano-lite-x86-nemu)

target_sources(nano-lite-x86-nemu
  PRIVATE
  src/syscall.c
  src/ramdisk.c
  src/proc.c
  src/fs.c
  src/irq.c
  src/loader.c
  src/mm.c
  src/device.c
  src/main.c)

# target_include_directories(nano-lite-x86-nemu
#   PRIVATE
#   "../nexus-am/am"
#   "../nexus-am/libs/klib/include")

target_include_directories(nano-lite-x86-nemu
  PUBLIC "include")

target_link_libraries(nano-lite-x86-nemu
  am klib)
