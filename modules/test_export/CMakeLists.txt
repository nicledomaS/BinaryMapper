cmake_minimum_required(VERSION 3.7)

project(test_export)

set(CMAKE_CXX_STANDARD 14)

file(GLOB SOURCES "src/*")

add_library(${PROJECT_NAME} SHARED ${INCLUDES} ${SOURCES})

target_link_libraries(${PROJECT_NAME} dl)
