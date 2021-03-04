cmake_minimum_required(VERSION 3.7)

project(test_export)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/modules/common/include)

file(GLOB SOURCES "src/*")
file(GLOB INCLUDES ${COMMON_INCLUDE_DIRS}/*.h)

add_library(${PROJECT_NAME} SHARED ${INCLUDES} ${SOURCES})

target_link_libraries(${PROJECT_NAME} dl)
