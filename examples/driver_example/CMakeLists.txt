cmake_minimum_required(VERSION 3.10)
project(driver_example)

set(KERNEL_DIR "" CACHE STRING "Path to the kernel build directory")
if("${KERNEL_DIR}" STREQUAL "")
  execute_process(COMMAND uname -r OUTPUT_VARIABLE uname_r
                  OUTPUT_STRIP_TRAILING_WHITESPACE)
  set(KERNEL_DIR "/lib/modules/${uname_r}/build")
endif()

set(COMMON_LIB_NAME "common")

file(GLOB LIB_SOURCES lib/*.c)
add_library(${COMMON_LIB_NAME} STATIC ${LIB_SOURCES})
get_target_property(ALL_MODULE_SOURCES ${COMMON_LIB_NAME} SOURCES)

file(GLOB MODULES_SOURCES *.c)
list(APPEND ALL_MODULE_SOURCES ${MODULES_SOURCES})

string(REPLACE ";" " " MODULE_SOURCES_STRING "${ALL_MODULE_SOURCES}")
configure_file(Kbuild.in Kbuild @ONLY)

foreach(src ${ALL_MODULE_SOURCES})
  configure_file(${src} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
endforeach()

set(MODULE_CMD ${CMAKE_MAKE_PROGRAM} -C ${KERNEL_DIR} M=${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(OUTPUT ${PROJECT_NAME}.ko
  COMMAND ${MODULE_CMD} modules
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${ALL_MODULE_SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/Kbuild
  VERBATIM)
add_custom_target(${PROJECT_NAME} ALL DEPENDS ${PROJECT_NAME}.ko)
add_custom_target(module-clean COMMAND ${MODULE_CMD} clean)
