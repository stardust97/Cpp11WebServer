set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

aux_source_directory(${PROJECT_SOURCE_DIR}/src/base BASE_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/util UTIL_LIST)
aux_source_directory(. SRC_LIST)

add_executable(test ${SRC_LIST} ${UTIL_LIST} ${BASE_LIST})

target_link_libraries( test PRIVATE log4cxx pthread)