aux_source_directory(. SRC_LIST)


add_executable(service ${SRC_LIST} ${UTIL_LIST} ${BASE_LIST})

target_link_libraries( service PRIVATE log4cxx pthread)