# Root CMakeLists.txt for the Calculator project

cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Calculator VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp)

# Add the src directory
add_subdirectory(lib)

# Specify the executable target
add_executable(calculator ${SRC_FILES})

# Link the src library to the calculator executable
target_link_libraries(calculator PRIVATE imgui)