cmake_minimum_required(VERSION 3.02)
project(autoreflect)

set (CMAKE_CXX_STANDARD 11)
add_executable(autoreflect generator/main.cpp)
if (MSVC)
    add_definitions(
        -D_CRT_SECURE_NO_WARNINGS
    )
endif()