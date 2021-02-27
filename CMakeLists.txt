cmake_minimum_required(VERSION 3.7)

project(BinaryMapper)

include(cmake/make.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

static_library(${PROJECT_NAME} SOURCES "src/")

add_subdirectory(example)
