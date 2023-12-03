project(Day03)

include_directories("../mdspan/include")

add_executable(day03
    day03.cpp
    day03main.cpp)
target_compile_definitions(day03 PRIVATE DOCTEST_CONFIG_DISABLE)

add_executable(day03test
    day03.cpp
    day03test.cpp)

message(DEBUG ${CMAKE_BUILD_TYPE})

file(GLOB TEXT_FILES *.txt)

file(COPY ${TEXT_FILES} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
