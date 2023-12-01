project(Day01)

add_executable(day01 day01.cpp)

message(DEBUG ${CMAKE_BUILD_TYPE})

# FIXME: Is there a way to get the actual .exe output dir on multi-config generator?
file(COPY example.txt DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Debug")
