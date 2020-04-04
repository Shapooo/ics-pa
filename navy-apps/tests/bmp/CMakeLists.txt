project(bmp)
add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
  libc
  libos
  libndl
  )
