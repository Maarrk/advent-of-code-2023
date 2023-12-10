project(Day03)

include_directories("../mdspan/include")

add_executable(day10
    day10.cpp
    day10main.cpp)
target_compile_definitions(day10 PRIVATE DOCTEST_CONFIG_DISABLE)

add_executable(day10test
    day10.cpp
    day10test.cpp)

message(DEBUG ${CMAKE_BUILD_TYPE})

file(GLOB TEXT_FILES *.txt)

file(COPY ${TEXT_FILES} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
