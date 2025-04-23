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

# SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

set(FONT_INPUT ${CMAKE_CURRENT_SOURCE_DIR}/fonts/DejaVuSans.ttf)
set(FONT_TEMP ${CMAKE_CURRENT_BINARY_DIR}/DejaVuSans.ttf)
set(FONT_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/dejavusans_ttf.h)

# Step 1: Copy the font to the build dir with a simple name
add_custom_command(
    OUTPUT ${FONT_TEMP}
    COMMAND ${CMAKE_COMMAND} -E copy ${FONT_INPUT} ${FONT_TEMP}
    DEPENDS ${FONT_INPUT}
    COMMENT "Copying font to build dir for clean xxd output"
)

# Step 2: Generate the header from the copied file
add_custom_command(
    OUTPUT ${FONT_OUTPUT}
    COMMAND xxd -i DejaVuSans.ttf > dejavusans_ttf.h
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${FONT_TEMP}
    COMMENT "Embedding DejaVuSans.ttf as header"
)

add_custom_target(embed_fonts DEPENDS ${FONT_OUTPUT})
add_dependencies(calculator embed_fonts)

target_include_directories(calculator PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# Link the src library to the calculator executable
target_link_libraries(calculator PRIVATE
    ${SDL2_LIBRARIES}
    imgui)