cmake_minimum_required(VERSION 3.20)

project(AdventOfCode2023)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories("doctest/doctest")

add_subdirectory(day01)
add_subdirectory(day02)
add_subdirectory(day03)
add_subdirectory(day04)
add_subdirectory(day05)
add_subdirectory(day10)
add_subdirectory(day11)
add_subdirectory(day22)
