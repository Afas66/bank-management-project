cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(BankManagementSystem VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/project2.cpp
)

# Add executable
add_executable(BankManagementSystem ${SOURCES})

# Optionally, you can add tests here if you have a testing framework set up
# enable_testing()
# add_subdirectory(tests)