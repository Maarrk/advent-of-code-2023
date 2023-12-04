#include <algorithm>
#include <charconv>
#include <format>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

#include "doctest.h"

#include "day03.h"

using namespace std::literals;

char_mat load_istream(std::istream &in) {
    // mdarray seems to only accept vector as container
    std::vector<char> container{};

    std::string line{};
    int line_number = 0;
    std::optional<size_t> line_length{};
    while (std::getline(in, line)) {
        line_number++;
        if (line_length) {
            if (line.length() != *line_length) {
                throw std::invalid_argument{
                    std::format("Line {} with length {} different than first "
                                "line length ({})",
                                line_number, line.length(), *line_length)};
            }
        } else {
            if (line.length() == 0) {
                throw std::invalid_argument{"Empty input line"};
            }
            line_length = line.length();
        }

        for (auto &&c : line) {
            container.push_back(c);
        }
    }

    if (line_length) {
        size_t rows = container.size() / *line_length;
        return char_mat{std::dextents<char, 2>{rows, *line_length}, container};
    } else {
        return char_mat{};
    }
}

TEST_CASE("loading from stream") {
    {
        std::istringstream inbuf_bad{"123\n12"};
        CHECK_THROWS(load_istream(inbuf_bad));
        std::istringstream inbuf_ok{"123\n123"};
        CHECK_NOTHROW(load_istream(inbuf_ok));
        std::istringstream inbuf_zero{"\n\n"};
        CHECK_THROWS(load_istream(inbuf_zero));
    }
    {
        std::istringstream inbuf{""};
        auto mat = load_istream(inbuf);
        CHECK_EQ(mat.size(), 0);
    }
    {
        std::istringstream inbuf{"a"};
        auto mat = load_istream(inbuf);
        CHECK_EQ(mat.size(), 1);
        CHECK_EQ(mat(0, 0), 'a');
    }
    {
        std::istringstream inbuf{"abc\ndef"};
        auto mat = load_istream(inbuf);
        CHECK_EQ(mat.size(), 6);
        CHECK_EQ(mat(0, 0), 'a');
        CHECK_EQ(mat(1, 1), 'e');
        CHECK_EQ(mat(0, 2), 'c');
    }
}
