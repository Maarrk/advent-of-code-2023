#include <ostream>
#include <string>

#include "raylib-cpp.hpp"

namespace rl = raylib;

struct Brick {
    rl::Vector3 start;
    rl::Vector3 end;

    Brick(rl::Vector3 v1, rl::Vector3 v2);
    static Brick from_string(const std::string &input_line);

    bool operator==(const Brick &) const = default;

    void Draw() const;
};

std::ostream &operator<<(std::ostream &os, const Brick &brick);