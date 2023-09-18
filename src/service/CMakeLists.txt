aux_source_directory(. SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/util UTIL_LIST)

add_executable(service ${SRC_LIST} ${UTIL_LIST})