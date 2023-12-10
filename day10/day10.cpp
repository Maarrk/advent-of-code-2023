#include <format>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <type_traits>

#include "doctest.h"

#include "day10.h"

using namespace std::literals;

// It seems that static analysis in VS Code doesn't recognize functions with
// type alias, suggesting to add their implementation
using Con = Connections;
using Con_t = std::underlying_type_t<Connections>;

constexpr Con operator~(Con a) {
    // Invert only four bits
    return static_cast<Con>(static_cast<Con_t>(a) ^
                            static_cast<Con_t>(Con::All));
}
constexpr Con operator|(Con a, Con b) {
    return static_cast<Con>(static_cast<Con_t>(a) | static_cast<Con_t>(b));
}
constexpr Con operator&(Con a, Con b) {
    return static_cast<Con>(static_cast<Con_t>(a) & static_cast<Con_t>(b));
}
constexpr Con operator^(Con a, Con b) {
    return static_cast<Con>(static_cast<Con_t>(a) ^ static_cast<Con_t>(b));
}
Con &operator|=(Con &a, Con b) {
    return (Con &)((Con_t &)a |= static_cast<Con_t>(b));
}
Con &operator&=(Con &a, Con b) {
    return (Con &)((Con_t &)a &= static_cast<Con_t>(b));
}
Con &operator^=(Con &a, Con b) {
    return (Con &)((Con_t &)a ^= static_cast<Con_t>(b));
}
constexpr Con operator-(Con a) {
    switch (a) {
    // invert vertically
    case Con::N:
    case Con::S:
    // case Con::N | Con::EW:
    // case Con::S | Con::EW:
        return a ^ Con::NS;

    // invert horizontally
    case Con::E:
    case Con::W:
    // case Con::E | Con::NS:
    // case Con::W | Con::NS:
        return a ^ Con::EW;

    // invert diagonally
    case Con::NE:
    case Con::NW:
    case Con::SE:
    case Con::SW:
        return a ^ Con::All;

    // same result
    case Con::None:
    case Con::All:
    case Con::NS:
    case Con::EW:
        return a;
    }
}

TEST_CASE("Connections bitwise operations") {
    using Con = Connections;
    CHECK_EQ(Con::N | Con::E, Con::NE);
    CHECK_EQ(~Con::NW, Con::SE);
    Con con{Con::NS};
    con &= Con::S;
    CHECK_EQ(con, Con::S);

    CHECK_EQ(-Con::N, Con::S);
    CHECK_EQ(-Con::S, Con::N);
    CHECK_EQ(-Con::E, Con::W);
    CHECK_EQ(-Con::NW, Con::SE);
}

Cell::Cell(char c) {
    const auto cm = char_mapping();
    auto it = std::ranges::find(cm, c);
    if (it == cm.end())
        throw std::invalid_argument{std::format(
            "unrecognized character '{}', only '|-LJ7F.S' allowed", c)};
    possible = static_cast<Connections>(std::distance(cm.begin(), it));
}

void Cell::add_links(Connections neighbors) {
    if (!is_valid_linked(linked | neighbors)) {
        auto cm = char_mapping();
        throw std::logic_error{
            std::format("Cell with shape '{}' cannot accept link '{}'",
                        cm[(int)possible], cm[(int)neighbors])};
    }
    linked |= neighbors;
}

TEST_CASE("cell methods") {
    Cell def{};
    CHECK_EQ(def.get_possible(), Con::None);
    CHECK_FALSE(def.start_distance);

    Cell nw{'J'};
    CHECK_EQ(nw.get_possible(), Con::NW);

    CHECK_THROWS(Cell{'X'});
}

constexpr std::array<char, static_cast<size_t>(Con::All) + 1>
Cell::char_mapping() {
    const size_t len = static_cast<size_t>(Con::All) + 1;
    std::array<char, len> arr{};
    arr.fill(0);
    struct M {
        char c;
        Con con;
    };
    std::array mappings{
        // |-LJ7F.S
        M{'|', Con::NS}, M{'-', Con::EW}, M{'L', Con::NE},   M{'J', Con::NW},
        M{'7', Con::SW}, M{'F', Con::SE}, M{'.', Con::None}, M{'S', Con::All},
    };
    for (auto &&m : mappings) {
        arr[static_cast<size_t>(m.con)] = m.c;
    }

    return arr;
}

constexpr bool Cell::is_valid_possible(Connections con) {
    auto cm = char_mapping();
    return cm[static_cast<size_t>(con)] != 0;
}

bool Cell::is_valid_linked(Connections con) {
    // Is a subset of possible connections?
    return (con | possible) == possible;
};

std::pair<std::vector<Cell>, Cell_mat> load_mat(std::istream &in) {
    std::vector<Cell> storage{};
    std::optional<size_t> row_length;
    size_t row = 1;
    size_t column = 0;
    while (in) {
        char c;
        in.get(c);
        if (!in) {
            break;
        }

        if (c == '\r') {
            continue; // skips column increment on purpose
        } else if (c == '\n') {
            if (!row_length) {
                if (column == 0) {
                    throw std::invalid_argument{"empty rows not allowed"};
                }
                row_length = column;
            } else if (column != row_length) {
                throw std::invalid_argument{std::format(
                    "length {} of row {} different than expected {}", column,
                    row, *row_length)};
            }
            row++;
            column = 0;
            continue; // skip incrementing column
        } else {
            storage.push_back(Cell(c));
        }

        column++;
    }

    if (!row_length) { // everything in single line
        row_length = column;
    }

    if (column != *row_length)
        throw std::invalid_argument{std::format(
            "last row only had {} chars, expected {}", column, *row_length)};

    return {storage, Cell_mat(storage.data(), row, column)};
}

TEST_CASE("loading cells") {
    {
        std::istringstream in{R"(.....
.F-7.
.|.|.
.L-J.
.....)"};
        auto [store, mat] = load_mat(in);
        CHECK_EQ(store.size(), 25);
        CHECK_EQ(mat.extent(0), 5);
        CHECK_EQ(mat.extent(1), 5);
    }
}