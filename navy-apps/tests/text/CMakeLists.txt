project(text)
add_executable(${PROJECT_NAME} text.c)

target_link_libraries(${PROJECT_NAME}
  libc
  libos
  libndl
  )
