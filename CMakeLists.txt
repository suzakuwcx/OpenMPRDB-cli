cmake_minimum_required(VERSION 3.21)
project(OpenMPRDB_cli)

message(${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_CXX_STANDARD 11)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_CXX_FLAGS "")

add_subdirectory(src)
add_subdirectory(test)