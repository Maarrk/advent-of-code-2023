#include <array>
#include <string>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "day03.h"

TEST_CASE("mdspan basics") {
    std::array input{
        // comments after rows prevent formatter from collapsing to one line
        1, 2, 3, //
        4, 5, 6, //
        7, 8, 9, //
    };
    stdex::mdspan m{input.data(), stdex::extents{3, 3}};

    CHECK_EQ(m(0, 0), 1);
    CHECK_EQ(m(2, 0), 7);
}

TEST_CASE("mdspan with string") {
    std::string input{"abcdef"};
    size_t line_length = 3;
    stdex::mdspan m{input.data(), input.length() / line_length, line_length};

    CHECK_EQ(m(0, 0), 'a');
    CHECK_EQ(m(1, 1), 'e');
    CHECK_EQ(m(0, 2), 'c');
}