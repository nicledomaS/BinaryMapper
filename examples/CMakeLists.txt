add_subdirectory(example)
add_subdirectory(test_exec)
add_subdirectory(test_export)
add_subdirectory(driver_example)

file(COPY ${CMAKE_CURRENT_LIST_DIR}/sh_example DESTINATION ${BinaryMapper_BINARY_DIR}/examples)
file(COPY ${CMAKE_CURRENT_LIST_DIR}/py_example DESTINATION ${BinaryMapper_BINARY_DIR}/examples)