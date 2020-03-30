cmake_minimum_required(VERSION 3.16)

project(nexus-all)

# list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# include(check)

add_subdirectory(am)

add_subdirectory(libs/klib)

add_subdirectory(tests/amtest)

add_subdirectory(tests/cputest)

# add_subdirectory(tests/cachetest)
