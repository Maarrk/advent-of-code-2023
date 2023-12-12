#include <algorithm>
#include <cstdlib>
#include <format>
#include <numeric>
#include <optional>
#include <sstream>
#include <tuple>

#include "doctest.h"

#include "day11.h"

int64_t Galaxy::distance(const Galaxy &other) const {
    return std::abs(row - other.row) + std::abs(col - other.col);
}

std::ostream &operator<<(std::ostream &os, const Galaxy &galaxy) {
    return os << "G{" << galaxy.row << ", " << galaxy.col << "}";
}

std::vector<Galaxy> load(std::istream &in) {
    std::vector<Galaxy> by_row{};
    std::optional<int64_t> row_length;
    int row = 0;
    int col = 0;
    char c;
    while (in.get(c) && !in.eof()) {
        // first one returns stream itself, which converts to !fail

        if (c == '\r') {
            continue; // skips column increment on purpose
        } else if (c == '\n') {
            if (!row_length) {
                if (col == 0) {
                    throw std::invalid_argument{"empty rows not allowed"};
                }
                row_length = col;
            } else if (col != row_length) {
                throw std::invalid_argument{
                    std::format("length {} of row {} (0-indexed) different "
                                "than expected {}",
                                col, row, *row_length)};
            }
            row++;
            col = 0;
            continue; // skip incrementing column
        } else if (c == '#') {
            by_row.emplace_back(row, col);
        } else if (c != '.') {
            throw std::invalid_argument{std::format(
                "unexpected char '{}' at row {}, column {} (0-indexed)"
                R"(, only '.', '#', '\n', and '\r' allowed)",
                c, row, col)};
        }

        col++;
    }

    if (!row_length) { // everything in single line
        row_length = col;
    }

    if (col != *row_length)
        throw std::invalid_argument{std::format(
            "last row only had {} chars, expected {}", col, *row_length)};

    return by_row;
}

TEST_CASE("trivial") {
    std::istringstream in{R"(
#..
..#
.#.)"};
    in.seekg(1); // skip the initial newline for nicer formatting
    auto by_row = load(in);
    CHECK_EQ(by_row, std::vector<Galaxy>{{0, 0}, {1, 2}, {2, 1}});
    auto by_col = sorted_by_col(by_row);
    CHECK_EQ(by_col, std::vector<size_t>{0, 2, 1});
}

std::vector<size_t> sorted_by_col(const std::vector<Galaxy> &universe) {
    std::vector<size_t> by_col{};
    by_col.reserve(universe.size());
    for (size_t i = 0; i < universe.size(); i++) {
        by_col.push_back(i);
    }

    const auto cmp_col_row = [&universe](const size_t &a,
                                         const size_t &b) -> bool {
        // should be a < b for ascending
        auto ga = universe[a];
        auto gb = universe[b];
        auto ta = std::make_tuple(ga.col, ga.row);
        auto tb = std::make_tuple(gb.col, gb.row);
        return ta < tb;
    };
    std::ranges::sort(by_col, cmp_col_row);

    return by_col;
}

void expand_universe(std::vector<Galaxy> &by_row,
                     const std::vector<size_t> &by_col, int64_t factor) {
    { // grow rows first
        int64_t last_row = 0;
        int64_t growth = 0;
        for (auto &galaxy : by_row) {
            int64_t empty_rows = galaxy.row - last_row - 1;
            // grow by the calculated amount but don't shrink
            growth += std::max(0LL, empty_rows * factor - 1);
            last_row = galaxy.row; // still original coordinates here
            galaxy.row += growth;
        }
    }
    { // grow columns now
        int64_t last_col = 0;
        int64_t growth = 0;
        for (auto galaxy_index : by_col) {
            auto &galaxy = by_row[galaxy_index];
            int64_t empty_cols = galaxy.col - last_col - 1;
            growth += std::max(0LL, empty_cols * (factor - 1));
            last_col = galaxy.col;
            galaxy.col += growth;
        }
    }
}

TEST_CASE("expansion validation") {
    std::vector<Galaxy> initial{{0, 0}, {0, 3}};
    auto by_col = sorted_by_col(initial);
    auto grow2{initial};
    expand_universe(grow2, by_col, 2);
    CHECK_EQ(grow2[0], Galaxy{0, 0}); // sanity check
    CHECK_EQ(grow2[1], Galaxy{0, 5}); // multiple columns
}

TEST_CASE("site example") {
    std::vector<Galaxy> initial{{0, 3}, {1, 7}, {2, 0}, {4, 6}, {5, 1},
                                {6, 9}, {8, 7}, {9, 0}, {9, 4}};
    auto by_col = sorted_by_col(initial); // sorting won't change
    auto grow2{initial};
    expand_universe(grow2, by_col, 2);
    CHECK_EQ(grow2[2], Galaxy{2, 0});  // unchanging
    CHECK_EQ(grow2[0], Galaxy{0, 4});  // move right by one
    CHECK_EQ(grow2[6], Galaxy{10, 9}); // move by two in both directions

    auto dist2 = closest_distances(grow2);
    int64_t sum2 = std::accumulate(dist2.begin(), dist2.end(), 0);
    CHECK_EQ(sum2, 374);

    auto grow10{initial};
    expand_universe(grow10, by_col, 10);
    auto dist10 = closest_distances(grow10);
    int64_t sum10 = std::accumulate(dist10.begin(), dist10.end(), 0);
    CHECK_EQ(sum10, 1030);

    auto grow100{initial};
    expand_universe(grow100, by_col, 100);
    auto dist100 = closest_distances(grow100);
    int64_t sum100 = std::accumulate(dist100.begin(), dist100.end(), 0);
    CHECK_EQ(sum100, 8410);
}

std::vector<int64_t> closest_distances(const std::vector<Galaxy> &galaxies) {
    std::vector<int64_t> closest{};

    // combinations 1-2, 1-3, 1-4, 2-3, 2-4, 3-4
    for (auto low_it = galaxies.begin();
         std::distance(low_it, galaxies.end()) > 1; low_it++) {
        // when entering here, there is definitely space to increment low_it
        for (auto high_it = low_it + 1; high_it != galaxies.end(); high_it++) {
            closest.push_back((*low_it).distance(*high_it));
        }
    }

    // formula for unordered pair count of N items
    CHECK_EQ(closest.size(), galaxies.size() * (galaxies.size() - 1) / 2);

    return closest;
}