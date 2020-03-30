project(amtest)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
  PRIVATE
  ./src/main.c
  ./src/tests/vm.c
  ./src/tests/hello.c
  ./src/tests/video.c
  ./src/tests/rtc.c
  ./src/tests/mp.c
  ./src/tests/intr.c
  ./src/tests/keyboard.c
  ./src/tests/devscan.c
  )

target_include_directories(${PROJECT_NAME}
  PRIVATE include)

target_link_libraries(${PROJECT_NAME}
  PRIVATE am klib)
