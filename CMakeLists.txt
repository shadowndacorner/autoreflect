cmake_minimum_required(VERSION 3.02)
project(autoreflect)

add_executable(generator generator/main.cpp)
if (MSVC)
    add_definitions(
        -D_CRT_SECURE_NO_WARNINGS
    )
endif()