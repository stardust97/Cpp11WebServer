cmake_minimum_required(VERSION 3.10)
project(webserver VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

add_definitions("-Wall -g")

find_package(log4cxx)


include_directories(
  ${PROJECT_SOURCE_DIR}/include
)
link_directories(
  /usr/local/lib
)

add_subdirectory(${PROJECT_SOURCE_DIR}/src)