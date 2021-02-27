cmake_minimum_required(VERSION 3.10)

project(example)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

executable(${PROJECT_NAME}
            SOURCES "./"
            THREADS
            MODULES BinaryMapper)

target_link_libraries(${PROJECT_NAME} dl)
