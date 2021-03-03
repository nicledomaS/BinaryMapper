cmake_minimum_required(VERSION 3.7)

project(BinaryMapper)

set(CMAKE_CXX_STANDARD 17)

include(cmake/make.cmake)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

include_directories(${CMAKE_SOURCE_DIR}/modules/common/include)

static_library(${PROJECT_NAME} SOURCES "src/")

add_subdirectory(examples)
