project(am)

add_library(${PROJECT_NAME} STATIC)

set(ISA x86 CACHE STRING "ISA")

# set(ARCH X86-NEMU CACHE )

# set(UP_ARCH)
set(ARCH x86-nemu CACHE STRING "ARCH")

target_compile_definitions(${PROJECT_NAME}
  PUBLIC -D__ISA__=\"${ISA}\" -D__ARCH__=${ARCH}
  PUBLIC -DARCH_H=\"arch/${ARCH}.h\"
  PUBLIC -DISA_H=\"${ISA}.h\")

target_compile_definitions(${PROJECT_NAME}
  PUBLIC -D__ISA_X86__ -D__ARCH_X86_NEMU__)

target_include_directories(${PROJECT_NAME} PUBLIC ".")

target_sources(${PROJECT_NAME}
  PRIVATE
  src/x86/nemu/cte.c
  src/x86/nemu/vme.c
  src/x86/nemu/cte.c
  src/x86/nemu/vme.c
  src/nemu-common/nemu-timer.c
  src/nemu-common/nemu-video.c
  src/nemu-common/trm.c
  src/nemu-common/ioe.c
  src/nemu-common/nemu-input.c
  src/dummy/mpe.c
  )

target_sources(${PROJECT_NAME}
  PRIVATE
  src/x86/nemu/trap.S
  src/x86/nemu/boot/start.S)

target_include_directories(${PROJECT_NAME} PUBLIC include)
