cmake_minimum_required(VERSION 3.10)
project(webserver VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

add_definitions("-Wall -g")

include_directories(
  ${PROJECT_SOURCE_DIR}/include
)

add_subdirectory(${PROJECT_SOURCE_DIR}/src)