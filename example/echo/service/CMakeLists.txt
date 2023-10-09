aux_source_directory(. SRC_LIST)

add_executable(service ${SRC_LIST} ${UTIL_LIST} ${BASE_LIST})

target_include_directories(
  ${PROJECT_SOURCE_DIR}/example/echo/service
)