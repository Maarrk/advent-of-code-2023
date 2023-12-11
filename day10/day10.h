#include <cstdint>
#include <istream>
#include <optional>
#include <tuple>
#include <variant>
#include <vector>

#include <experimental/mdarray>

#include "doctest.h"

namespace stdex = std::experimental;

class Cell;
using Cell_mat = stdex::mdarray<Cell, std::dextents<size_t, 2>>;
using Location = std::pair<Cell_mat::index_type, Cell_mat::index_type>;

enum class Connections : uint8_t {
    None = 0x00,
    N = 1 << 0,
    S = 1 << 1,
    E = 1 << 2,
    W = 1 << 3,
    NE = N | E,
    NW = N | W,
    SE = S | E,
    SW = S | W,
    NS = N | S,
    EW = E | W,
    All = 0x0F,
};

/// Invert all connection state yes<->no
constexpr Connections operator~(Connections a);
/// Opposite connection (rotate by 180 deg)
constexpr Connections operator-(Connections a);
constexpr Connections operator|(Connections a, Connections b);
constexpr Connections operator&(Connections a, Connections b);
constexpr Connections operator^(Connections a, Connections b);
Connections &operator|=(Connections &a, Connections b);
Connections &operator&=(Connections &a, Connections b);
Connections &operator^=(Connections &a, Connections b);

class Cell {
  public:
    Cell() = default;
    Cell(const Cell &) = default;
    Cell &operator=(const Cell &) = default;
    Cell(Cell &&) = default;
    Cell &operator=(Cell &&) = default;
    explicit Cell(char c);

    bool operator==(const Cell &) const = default;

    std::variant<std::monostate, short, bool> usage{};
    enum Usage : size_t {
        None = 0,
        Distance = 1,
        Contained = 2,
    };

    Connections get_possible() const { return possible; };
    Connections get_linked() const { return linked; }
    void add_links(Connections neighbors);

  private:
    Connections possible{Connections::None};
    Connections linked{Connections::None};

    /// Single source of truth for converting Connection <-> char
    static constexpr std::array<char, static_cast<size_t>(Connections::All) + 1>
    char_mapping();

    /// Is any cell allowed to have this shape
    static constexpr bool is_valid_possible(Connections con);

    /// Is this cell allowed to have these links
    bool is_valid_linked(Connections con);
};

Cell_mat load_mat(std::istream &in);
void set_usage(Cell_mat &mat);
void _add_links(Cell_mat &mat);
void _count_distance(Cell_mat &mat, std::optional<Location> start = {});
void _mark_contained(Cell_mat &mat);