cmake_minimum_required(VERSION 3.8)
project(PolarCode)

set(CMAKE_CXX_STANDARD 11)

include_directories(src)
add_subdirectory(src)


if(NOT without-test)
    enable_testing()
    include(cmake/gtest.cmake)
    add_subdirectory(test)
endif()
