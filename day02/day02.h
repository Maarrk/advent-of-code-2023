#include <string_view>

struct CubeSet {
    int red{0};
    int green{0};
    int blue{0};

    static const CubeSet valid_game_limit;

    bool operator==(const CubeSet &) const = default;
    bool is_subset(const CubeSet &other) const;
    bool is_superset(const CubeSet &other) const {
        return other.is_subset(*this);
    }
    CubeSet set_union(const CubeSet &other) const;
    int power() const;
};

int parse_game_id(std::string_view line);
CubeSet game_required_set(std::string_view line);
