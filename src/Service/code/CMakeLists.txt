# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)

# Set the project name
project (rush)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Create a sources variable with a link to all cpp files to compile
set(SOURCES main.cpp Server.cpp)

link_directories("../../build/lib" "../../build/lib/http")

# Add an executable with the above sources
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../../Debug/)
add_executable(service ${SOURCES})

include_directories("../../lib/include/" "../../lib/http/include/")

target_link_libraries(service libevent.a libhttp.a)

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(service
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)