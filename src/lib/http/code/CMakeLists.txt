# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the project name
project (httplibTest)

# Create a sources variable with a link to all cpp files to compile
set(SOURCES
    src/httplib.cpp
    src/httpTest.cpp
    src/cookie.cpp
    src/headers.cpp
    src/request.cpp
    src/response.cpp
    src/uri.cpp
)

# Add an executable with the above sources
add_executable(httplibTest ${SOURCES})

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(httplibTest
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)