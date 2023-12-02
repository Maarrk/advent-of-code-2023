project(Day02)

add_executable(day02
    day02.cpp
    day02main.cpp)
target_compile_definitions(day02 PRIVATE DOCTEST_CONFIG_DISABLE)

add_executable(day02test
    day02.cpp
    day02test.cpp)

message(DEBUG ${CMAKE_BUILD_TYPE})

file(GLOB TEXT_FILES *.txt)

# FIXME: Is there a way to get the actual .exe output dir on multi-config generator?
file(COPY ${TEXT_FILES} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Debug")
