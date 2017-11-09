cmake_minimum_required(VERSION 3.8)
project(PolarCode)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES Channel.cpp GnCosetCode.cpp)
add_executable(polar_code ${SOURCE_FILES})