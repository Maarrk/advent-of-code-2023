cmake_minimum_required(VERSION 3.20)

project(AdventOfCode2023)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories("doctest/doctest")

add_subdirectory(day01)
add_subdirectory(day02)
