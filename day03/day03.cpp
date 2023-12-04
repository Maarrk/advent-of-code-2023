#include <algorithm>
#include <charconv>
#include <format>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_set>

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
        CHECK_EQ(mat.extent(0), 0);
        CHECK_EQ(mat.extent(1), 0);
    }
    {
        std::istringstream inbuf{"a"};
        auto mat = load_istream(inbuf);
        CHECK_EQ(mat.size(), 1);
        CHECK_EQ(mat(0, 0), 'a');
        CHECK_EQ(mat.extent(0), 1);
        CHECK_EQ(mat.extent(1), 1);
    }
    {
        std::istringstream inbuf{"abc\ndef"};
        auto mat = load_istream(inbuf);
        CHECK_EQ(mat.size(), 6);
        CHECK_EQ(mat(0, 0), 'a');
        CHECK_EQ(mat(1, 1), 'e');
        CHECK_EQ(mat(0, 2), 'c');
        CHECK_EQ(mat.extent(0), 2);
        CHECK_EQ(mat.extent(1), 3);
    }
    {
        const size_t count = 140;
        const auto line = std::string(count, '.');
        std::stringstream inbuf{};
        for (size_t i = 0; i < count; i++) {
            if (i != 0)
                inbuf << "\n";
            inbuf << line;
        }

        auto mat = load_istream(inbuf);
        CHECK_EQ(mat.size(), count * count);
        CHECK_EQ(mat(0, 0), '.');
        CHECK_EQ(mat.extent(0), count);
        CHECK_EQ(mat.extent(1), count);
    }
}

std::vector<char>::const_iterator row_end(const char_mat &mat,
                                          char_mat::index_type row) {
    // assumes rows are continuous in container
    static_assert(std::is_same_v<char_mat::layout_type, std::layout_right>);

    if (row >= mat.extent(0)) {
        throw std::invalid_argument{
            std::format("Row index {} out of bounds for matrix with {} rows",
                        row, mat.extent(0))};
    }

    char_mat::index_type index = (row + 1) * mat.extent(1);
    return mat.container().begin() + index;
}

mat_it it_at(const char_mat &mat, char_mat::index_type row,
             char_mat::index_type column) {
    // assumes rows are continuous in container
    static_assert(std::is_same_v<char_mat::layout_type, std::layout_right>);

    if (row >= mat.extent(0)) {
        throw std::invalid_argument{
            std::format("Row index {} out of bounds for matrix with {} rows",
                        row, mat.extent(0))};
    }
    if (column >= mat.extent(1)) {
        throw std::invalid_argument{std::format(
            "Column index {} out of bounds for matrix with {} columns", column,
            mat.extent(1))};
    }

    char_mat::index_type index = row * mat.extent(1) + column;
    return mat.container().begin() + index;
}

void iterate_adjacent(
    const char_mat &mat, mat_part rows, mat_part columns,
    std::function<bool(char_mat::element_type)> continue_predicate) {

    // done as int for signed subtraction
    char_mat::size_type min_row = std::max((int)0, (int)rows.first - 1);
    char_mat::size_type max_row = std::min(mat.extent(0) - 1, rows.second + 1);
    char_mat::size_type min_col = std::max((int)0, (int)columns.first - 1);
    char_mat::size_type max_col =
        std::min(mat.extent(1) - 1, columns.second + 1);

    for (char_mat::size_type r = min_row; r <= max_row; r++) {
        for (char_mat::size_type c = min_col; c <= max_col; c++) {
            if (!continue_predicate(mat(r, c))) {
                return;
            }
        }
    }
}

TEST_CASE("character matrix iteration") {
    std::vector<char> data_v{
        'a', 'b', 'c', 'd', 'e', //
        'f', 'g', 'h', 'i', 'j', //
        'k', 'l', 'm', 'n', 'o', //
        'p', 'q', 'r', 's', 't', //
    };
    char_mat mat{std::extents<size_t, 4, 5>{}, data_v};

    SUBCASE("row end iterator") {
        // Do I even understand iterators correctly?
        CHECK_EQ(data_v.end(), data_v.begin() + 20);

        CHECK_EQ(row_end(mat, 3), mat.container().end());
        CHECK_EQ(row_end(mat, 0), mat.container().begin() + 5);

        CHECK_THROWS(row_end(mat, -1));
        CHECK_THROWS(row_end(mat, 5));
    }

    SUBCASE("iterator at position") {
        CHECK_EQ(it_at(mat, 0, 0), mat.container().begin());
        CHECK_EQ(it_at(mat, 1, 0), mat.container().begin() + 5);
        CHECK_EQ(it_at(mat, 1, 2), mat.container().begin() + 7);

        CHECK_THROWS(it_at(mat, 4, 0));
        CHECK_THROWS(it_at(mat, 0, 5));
    }

    SUBCASE("iterating adjacent") {
        using char_set = std::unordered_set<char>;
        const auto from_str = [](std::string_view str) {
            char_set set{};
            for (auto &&c : str) {
                set.insert(c);
            }
            return set;
        };
        char_set visited{};
        const auto visit_all = [&visited](char c) {
            visited.insert(c);
            return true;
        };

        // top corner
        iterate_adjacent(mat, {0, 0}, {0, 0}, visit_all);
        CHECK_EQ(visited, char_set{'a', 'b', 'f', 'g'});
        visited.clear();

        // around one point
        iterate_adjacent(mat, {1, 1}, {1, 1}, visit_all);
        CHECK_EQ(visited, from_str("abcfghklm"));
        visited.clear();

        // bottom word "qrs"
        iterate_adjacent(mat, {3, 3}, {1, 3}, visit_all);
        CHECK_EQ(visited, from_str("klmnopqrst"));
        visited.clear();

        // outside corner
        iterate_adjacent(mat, {-1, -1}, {-1, -1}, visit_all);
        CHECK_EQ(visited, char_set{'a'});
        visited.clear();

        int visited_counter{0};
        const auto visit_four = [&visited_counter](char c) {
            if (++visited_counter >= 4)
                return false;
            return true;
        };

        iterate_adjacent(mat, {1, 2}, {1, 3}, visit_four);
        CHECK_EQ(visited_counter, 4);
        visited_counter = 0;

        // outside above
        iterate_adjacent(mat, {-1, -1}, {-1, 0}, visit_four);
        CHECK_EQ(visited_counter, 2);
        visited_counter = 0;
    }
}

bool is_symbol(char c) { return !(c >= '0' && c <= '9') && c != '.'; }
