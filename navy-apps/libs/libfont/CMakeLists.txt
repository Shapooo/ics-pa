project(libfont)

add_library(${PROJECT_NAME} STATIC)

target_include_directories(${PROJECT_NAME}
  PUBLIC include)

target_sources(${PROJECT_NAME}
  PRIVATE
  ./src/font.cpp
  )
