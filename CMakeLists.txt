cmake_minimum_required(VERSION 3.23)
project(InterpreteurC C)

set(CMAKE_C_STANDARD 11)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src/includes_h)

# Source files
set(SOURCES
    main.c
    src/c_files/file.c
    src/c_files/log.c
)

# Output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# Add executable
add_executable(prog ${SOURCES})
