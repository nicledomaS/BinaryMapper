cmake_minimum_required(VERSION 3.10)

project(test_exec)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

executable(${PROJECT_NAME} SOURCES "./")


