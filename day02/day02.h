#include <string_view>

struct CubeSet {
    int red{0};
    int green{0};
    int blue{0};

    bool operator==(const CubeSet &) const = default;
    bool is_subset(const CubeSet &other) const;
    bool is_superset(const CubeSet &other) const {
        return other.is_subset(*this);
    }
};

int parse_game_id(std::string_view line);
bool validate_game(std::string_view line);
