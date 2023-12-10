#include <array>
#include <string>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "day10.h"

TEST_CASE("mdspan basics") {
    std::array input{
        // comments after rows prevent formatter from collapsing to one line
        1, 2, 3, //
        4, 5, 6, //
        7, 8, 9, //
    };
    stdex::mdspan mds{input.data(), stdex::extents{3, 3}};

    CHECK_EQ(mds(0, 0), 1);
    CHECK_EQ(mds(2, 0), 7);

    auto top = std::submdspan(mds, 0, std::full_extent);
    CHECK_EQ(top.rank(), 1);
    CHECK_EQ(top.extent(0), 3);
    CHECK_EQ(top.extent(1), 3);
    CHECK_EQ(top.extent(2), 3);
    CHECK_EQ(top.size(), 3);
    CHECK_EQ(top(0), 1);
    CHECK_EQ(top(1), 2);
    CHECK_EQ(top(2), 3);

    // // Doesn't compile (tried on MSVC and clang):
    // auto center = std::submdspan(mds, std::full_extent, 1);
    // CHECK_EQ(center.size(), 3);
    // CHECK_EQ(center(0), 2);
    // CHECK_EQ(center(1), 5);
    // CHECK_EQ(center(2), 8);

    std::vector<int> input_v{};
    for (auto &&n : input) {
        input_v.push_back(n);
    }
    auto mda = stdex::mdarray<int, std::dextents<int, 2>>{
        std::extents<int, 3, 3>{}, input_v};
    CHECK_EQ(mda(0, 0), 1);
    CHECK_EQ(mda(2, 0), 7);
    CHECK_EQ(mda.extent(0), 3);
    CHECK_EQ(mda.extent(1), 3);
    CHECK_EQ(mda.rank(), 2);
}

TEST_CASE("mdspan with string") {
    std::string input{"abcdef"};
    size_t line_length = 3;
    stdex::mdspan mds{input.data(), input.length() / line_length, line_length};

    CHECK_EQ(mds(0, 0), 'a');
    CHECK_EQ(mds(1, 1), 'e');
    CHECK_EQ(mds(0, 2), 'c');
    CHECK_EQ(mds.extent(0), 2);
    CHECK_EQ(mds.extent(1), 3);
}