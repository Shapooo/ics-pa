project(navy-dummy)
add_executable(${PROJECT_NAME} dummy.c)

target_link_libraries(${PROJECT_NAME}
  libc
  libfont
  libos
  libndl)
