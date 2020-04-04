project(hello)
add_executable(${PROJECT_NAME} hello.c)

target_link_libraries(${PROJECT_NAME}
  libc
  libos
  libndl
  )
