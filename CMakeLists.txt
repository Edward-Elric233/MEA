cmake_minimum_required(VERSION 3.20)
project(MEA)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)
include_directories(class)
include_directories(lib)

set(CMAKE_CXX_FLAGS "-static-libgcc -static-libstdc++ ${CMAKE_CXX_FLAGS}")

file(GLOB src_file src/*.cpp)
file(GLOB class_file class/*.cpp)

add_executable(MEA ${src_file} ${class_file} class/Random.cpp class/Random.h class/Depend.cpp class/Depend.h)
