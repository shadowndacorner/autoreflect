cmake_minimum_required(VERSION 3.02)
project(autoreflect_sample)

# TODO: Make this better/cross platform
set(GENERATOR_EXE ../build/Win64/Debug/generator.exe)
set(GENERATED_DIR "../generated/")

file(GLOB_RECURSE GENERATED_FILES
    "${GENERATED_DIR}/*.h"
    "${GENERATED_DIR}/*.hpp"
    "${GENERATED_DIR}/*.cpp"
)

add_executable(sample ${GENERATED_FILES} sample.cpp)
target_include_directories(sample PUBLIC ${GENERATED_DIR})
target_include_directories(sample PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set(CEREAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/cereal/include)
if (EXISTS ${CEREAL_DIR})
    add_definitions(-DWITH_CEREAL)
    target_include_directories(sample PUBLIC ${CEREAL_DIR})
endif()