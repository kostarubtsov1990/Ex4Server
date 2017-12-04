cmake_minimum_required(VERSION 3.8)
project(Ex4Server)

set(CMAKE_CXX_STANDARD 98)

set(SOURCE_FILES main.cpp Server.cpp Server.h)
add_executable(Ex4Server ${SOURCE_FILES})