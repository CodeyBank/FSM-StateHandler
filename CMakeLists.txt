cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(ProTimer VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Add the src directory
add_subdirectory(src)

# Find the ncurses package
find_package(Curses REQUIRED)

# Include the ncurses headers
include_directories(${CURSES_INCLUDE_DIR})

# Collect all source files (including header-only .hpp)
file(GLOB SOURCES
    "src/*.cpp"
    "src/*.hpp"  # Include header-only .hpp files
)

#Add the executable
add_executable(ProTimer-SH ${SOURCES})

#Link the ncurses library
target_link_libraries(ProTimer-SH ${CURSES_LIBRARIES})
