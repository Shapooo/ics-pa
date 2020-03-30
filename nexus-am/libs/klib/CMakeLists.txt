project(klib)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
  PRIVATE
  src/cpp.c
  src/int64.c
  src/intlib.h
  src/io.c
  src/stdio.c
  src/stdlib.c
  src/string.c
  )

target_include_directories(${PROJECT_NAME}
  PUBLIC
  include)

target_link_libraries(${PROJECT_NAME}
  am)
