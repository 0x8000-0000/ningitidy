cmake_minimum_required(VERSION 3.18)
project(
  ningitidy
  VERSION 0.0.1
  LANGUAGES CXX
  DESCRIPTION "Clang-Tidy accelerator")

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_CXX_EXTENSIONS False)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS NO_OUTPUT_DIRS)

find_package(Threads)

find_package(fmt CONFIG REQUIRED)
find_package(absl CONFIG REQUIRED)
find_package(re2 CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)

add_library(project_options INTERFACE)

add_library(project_warnings INTERFACE)
target_compile_options(project_warnings INTERFACE -Wall -Wextra -Werror
                                                  -pedantic -Wunused -Wshadow)

add_subdirectory(src)
