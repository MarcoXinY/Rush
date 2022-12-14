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
set(SOURCES main.cpp Server.cpp Chat.cpp JsonMessage.cpp)

link_directories(${CMAKE_SOURCE_DIR}/build/src/lib
                 ${CMAKE_SOURCE_DIR}/build/src/lib/http 
                 ${CMAKE_SOURCE_DIR}/build/src/lib/json
                 ${CMAKE_SOURCE_DIR}/build/src/lib/webSocket)

# Add an executable with the above sources
add_executable(service ${SOURCES})

include_directories("../../lib/include/"
                    "../../lib/http/include/"
                    "../../lib/json/include/"
                    "../../lib/webSocket/include/")

target_link_libraries(service libpool.a libhttp.a libjson.a libwebSocket.a)

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(service
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)