# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)

# Set the project name
project (rush)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(http)
add_subdirectory(json)

add_library(pool eventPoll.cpp threadPool.cpp)

target_include_directories(pool
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
)