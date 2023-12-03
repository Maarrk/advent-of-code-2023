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

// Would be nice to change into mdarray to own data
load_istream_result load_istream(std::istream &in, std::string &container) {
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
            line_length = line.length();
        }
        container.append(line);
    }

    if (line_length) {
        return load_istream_result{
            container,
            stdex::mdspan{container.data(), container.length() / *line_length,
                          *line_length},
        };
    } else {
        return load_istream_result{
            container,
            stdex::mdspan{container.data(), 0, 0},
        };
    }
}

TEST_CASE("loading from stream") {
    {
        std::string container_bad{};
        std::istringstream inbuf_bad{"123\n12"};
        CHECK_THROWS(load_istream(inbuf_bad, container_bad));
        std::string container_ok{};
        std::istringstream inbuf_ok{"123\n123"};
        CHECK_NOTHROW(load_istream(inbuf_ok, container_ok));
    }
    {
        std::istringstream inbuf{""};
        std::string container{};
        auto result = load_istream(inbuf, container);
        CHECK_EQ(result.text, ""sv);
        CHECK_EQ(result.mds.size(), 0);
    }
    {
        std::istringstream inbuf{"a"};
        std::string container{};
        auto result = load_istream(inbuf, container);
        CHECK_EQ(result.text, "a"sv);
        CHECK_EQ(result.mds.size(), 1);
        CHECK_EQ(result.mds(0, 0), 'a');
    }
    {
        std::istringstream inbuf{"abc\ndef"};
        std::string container{};
        auto result = load_istream(inbuf, container);
        CHECK_EQ(result.mds.size(), 6);
        CHECK_EQ(result.mds(0, 0), 'a');
        CHECK_EQ(result.mds(1, 1), 'e');
        CHECK_EQ(result.mds(0, 2), 'c');
    }
}
