set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/echo)
add_definitions("-Wall -g")

aux_source_directory(${PROJECT_SOURCE_DIR}/src/base BASE_LIST) // TODO 编译成链接库，去除源码的依赖
aux_source_directory(${PROJECT_SOURCE_DIR}/src/util UTIL_LIST)

add_subdirectory(client)
add_subdirectory(service)