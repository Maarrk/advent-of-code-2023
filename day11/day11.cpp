#include <algorithm>
#include <cstdlib>
#include <format>
#include <numeric>
#include <optional>
#include <sstream>
#include <tuple>

#include "doctest.h"

#include "day11.h"

int Galaxy::distance(const Galaxy &other) const {
    return std::abs(row - other.row) + std::abs(col - other.col);
}

std::ostream &operator<<(std::ostream &os, const Galaxy &galaxy) {
    return os << "G{" << galaxy.row << ", " << galaxy.col << "}";
}

std::vector<Galaxy> load(std::istream &in) {
    std::vector<Galaxy> by_row{};
    std::optional<int> row_length;
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
                     const std::vector<size_t> &by_col) {
    { // grow rows first
        int last_row = 0;
        int growth = 0;
        for (auto &galaxy : by_row) {
            // grow by one less than row difference, but don't shrink
            growth += std::max(0, galaxy.row - last_row - 1);
            last_row = galaxy.row; // still original coordinates here
            galaxy.row += growth;
        }
    }
    { // grow columns now
        int last_col = 0;
        int growth = 0;
        for (auto galaxy_index : by_col) {
            auto &galaxy = by_row[galaxy_index];
            growth += std::max(0, galaxy.col - last_col - 1);
            last_col = galaxy.col;
            galaxy.col += growth;
        }
    }
}

TEST_CASE("site example") {
    std::vector<Galaxy> by_row{{0, 3}, {1, 7}, {2, 0}, {4, 6}, {5, 1},
                               {6, 9}, {8, 7}, {9, 0}, {9, 4}};
    auto by_col = sorted_by_col(by_row);
    expand_universe(by_row, by_col);
    CHECK_EQ(by_row[2], Galaxy{2, 0});  // unchanging
    CHECK_EQ(by_row[0], Galaxy{0, 4});  // move right by one
    CHECK_EQ(by_row[6], Galaxy{10, 9}); // move by two in both directions

    auto pairs = closest_pairs(by_row, by_col);
    int distance_sum = std::accumulate(
        pairs.begin(), pairs.end(), 0,
        [&by_row](int acc, const auto &p) -> int {
            return acc + by_row[p.first].distance(by_row[p.second]);
        });
    CHECK_EQ(distance_sum, 374);
}

std::vector<std::pair<size_t, size_t>>
closest_pairs(const std::vector<Galaxy> &by_row,
              const std::vector<size_t> &by_col) {
    std::vector<std::pair<size_t, size_t>> closest{};

    CHECK_EQ(by_row.size(), by_col.size());
    const size_t count = by_row.size();

    // bci = by_col_index, bri = by_row_index
    for (size_t bci = 0; bci < count; bci++) {
        size_t bri = by_col[bci];
        Galaxy g = by_row[bri];

        // FIXME: Not enough pairs found
        // Probably because of skipping lower index neighbors?

        std::optional<int> min_dist;
        std::optional<size_t> other_bri;
        // optional is 'smaller' than any value, so to find minimal distance
        // look for maximal negated distance

        // both indexings are sorted spatially, so only check neighbors;
        // the way question is written should give same results for ties;

        // we only want ascending by_row indices in pair, so skip bri - 1;
        if (bri < count - 1) {
            Galaxy other = by_row[bri + 1];
            if (-g.distance(other) > min_dist) {
                min_dist = g.distance(other);
                other_bri = bri + 1;
            }
        }
        if (bci > 0 && by_col[bci - 1] > bri) {
            Galaxy other = by_row[by_col[bci - 1]];
            if (-g.distance(other) > min_dist) {
                min_dist = g.distance(other);
                other_bri = by_col[bci - 1];
            }
        }
        if (bci < count - 1 && by_col[bci + 1] > bri) {
            Galaxy other = by_row[by_col[bci + 1]];
            if (-g.distance(other) > min_dist) {
                min_dist = g.distance(other);
                other_bri = by_col[bci + 1];
            }
        }

        if (other_bri) { // can be empty with only one galaxy
            auto second = *other_bri;

            // assert logic in ifs above is correct for ascending pair
            CHECK_LT(bri, second);
            closest.emplace_back(bri, second);
        }
    }

    // formula for unordered pair count of N items
    CHECK_EQ(closest.size(), count * (count - 1) / 2);

    // just to make it nicer to read and compare
    // had to iterate by_col to have both indices
    std::ranges::sort(closest);
    return closest;
}