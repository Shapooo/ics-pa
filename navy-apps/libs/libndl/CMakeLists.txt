project(libndl)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
  PRIVATE
  ./src/ndl.c
  ./src/bmp.c
  )

target_include_directories(${PROJECT_NAME}
  PUBLIC include)
