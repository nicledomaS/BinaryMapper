cmake_minimum_required(VERSION 3.7)

project(RemoteBinaryMapper)

set(CMAKE_CXX_STANDARD 17)

include(cmake/make.cmake)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

include_directories(${CMAKE_SOURCE_DIR}/modules/common/include)

find_package(cpprestsdk REQUIRED)

executable(${PROJECT_NAME}
            SOURCES "./src"
            THREADS
            MODULES BinaryMapper)

target_link_libraries(${PROJECT_NAME} cpprestsdk::cpprest dl)

add_subdirectory(examples)
