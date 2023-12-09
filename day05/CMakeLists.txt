project(Day04)

add_executable(day05
    day05.cpp
    day05main.cpp)
target_compile_definitions(day05 PRIVATE DOCTEST_CONFIG_DISABLE)

add_executable(day05test
    day05.cpp
    day05test.cpp)

message(DEBUG ${CMAKE_BUILD_TYPE})

file(GLOB TEXT_FILES *.txt)

file(COPY ${TEXT_FILES} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
