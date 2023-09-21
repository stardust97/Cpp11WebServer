set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

aux_source_directory(${PROJECT_SOURCE_DIR}/src/base BASE_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/util UTIL_LIST)

add_subdirectory(client)
add_subdirectory(service)
