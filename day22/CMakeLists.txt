project(Day22)

add_executable(day22
    day22.cpp
    day22main.cpp)
target_compile_definitions(day22 PRIVATE DOCTEST_CONFIG_DISABLE)

add_executable(day22test
    day22.cpp
    day22test.cpp)

file(GLOB TEXT_FILES *.txt)

file(COPY ${TEXT_FILES} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
