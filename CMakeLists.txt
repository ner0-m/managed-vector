cmake_minimum_required(VERSION 3.16)

project(memory_management VERSION 0.0.1 LANGUAGES CXX)

include(cmake/CPM.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

CPMAddPackage("gh:fmtlib/fmt#7.1.3")

add_subdirectory(library)
add_subdirectory(examples)
add_subdirectory(tests)
