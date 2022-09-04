# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)

# Set the project name
project (json)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Create a sources variable with a link to all cpp files to compile
set(SOURCES
    src/json11.cpp
)

# Add an executable with the above sources
add_library(json ${SOURCES})

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(json
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)