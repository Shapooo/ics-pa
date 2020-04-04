project(events)
add_executable(${PROJECT_NAME} events.c)

target_link_libraries(${PROJECT_NAME}
  libc
  libos
  libndl
  )
