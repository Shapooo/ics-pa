project(libos)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
  PRIVATE
  ./src/nanos.c
  )

target_sources(${PROJECT_NAME}
  PRIVATE
  ./src/native.cpp
  )

target_compile_definitions(${PROJECT_NAME}
  PUBLIC -D__ISA_X86__)
