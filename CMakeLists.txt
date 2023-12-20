cmake_minimum_required(VERSION 3.23)
project(untitled C)

set(CMAKE_C_STANDARD 99)
add_subdirectory(Question1)
add_subdirectory(Question2)
add_subdirectory(Question3)
add_subdirectory(Question4)

add_executable(untitled Question3 Question1 Question2 Question4)
