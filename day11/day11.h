#include <iostream>
#include <tuple>
#include <vector>

struct Galaxy {
    int row;
    int col;

    int distance(const Galaxy &other) const;
    bool operator==(const Galaxy &) const = default;
};

std::ostream &operator<<(std::ostream &os, const Galaxy &galaxy);

std::vector<Galaxy> load(std::istream &in);
std::vector<size_t> sorted_by_col(const std::vector<Galaxy> &by_row);
void expand_universe(std::vector<Galaxy> &by_row,
                     const std::vector<size_t> &by_col);
/// sorted pairs of ascending indices into by_row for closest galaxies
std::vector<std::pair<size_t, size_t>>
closest_pairs(const std::vector<Galaxy> &by_row,
              const std::vector<size_t> &by_col);