project(Day03)

add_executable(day11
    day11.cpp
    day11main.cpp)
target_compile_definitions(day11 PRIVATE DOCTEST_CONFIG_DISABLE)

add_executable(day11test
    day11.cpp
    day11test.cpp)

message(DEBUG ${CMAKE_BUILD_TYPE})

file(GLOB TEXT_FILES *.txt)

file(COPY ${TEXT_FILES} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
