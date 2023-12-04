project(Day04)

add_executable(day04
    day04.cpp
    day04main.cpp)
target_compile_definitions(day04 PRIVATE DOCTEST_CONFIG_DISABLE)

add_executable(day04test
    day04.cpp
    day04test.cpp)

message(DEBUG ${CMAKE_BUILD_TYPE})

file(GLOB TEXT_FILES *.txt)

file(COPY ${TEXT_FILES} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
