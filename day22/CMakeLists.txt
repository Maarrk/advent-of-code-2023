project(Day22)

add_subdirectory(../raylib raylib)

include_directories(../raylib/src)
include_directories(../raylib-cpp/include)

add_executable(day22
    day22.cpp
    day22main.cpp)
target_compile_definitions(day22 PRIVATE DOCTEST_CONFIG_DISABLE)
target_link_libraries(day22 raylib)

add_executable(day22test
    day22.cpp
    day22test.cpp)
target_link_libraries(day22test raylib)

file(GLOB TEXT_FILES *.txt)

file(COPY ${TEXT_FILES} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
