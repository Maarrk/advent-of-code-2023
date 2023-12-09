#include <istream>
#include <map>
#include <set>

using num = long long;

// HACK: implemented in day05.cpp, too lazy to move templates to header
std::pair<num, num> solution(std::istream &input_file, bool verbose = false);

enum class Category {
    seed,
    soil,
    fertilizer,
    water,
    light,
    temperature,
    humidity,
    location,
};

std::ostream &operator<<(std::ostream &out, const Category &category);

int operator-(const Category &lhs, const Category &rhs);

template <typename T> struct Item {
    Category category;
    T number;

    bool operator==(const Item &) const = default;
};

template <typename T>
std::ostream &operator<<(std::ostream &out, const Item<T> &item);

template <typename T> struct Item_range {
    T destination_start;
    T source_start;
    T length;

    auto operator<=>(const Item_range &) const = default;
};

template <typename T>
std::istream &operator>>(std::istream &in, Item_range<T> &item_range);

template <typename T>
std::ostream &operator<<(std::ostream &out, const Item_range<T> &Item_range);

template <typename T> class Almanac_map;
template <typename T>
std::ostream &operator<<(std::ostream &out, const Almanac_map<T> &almanac_map);

template <typename T> class Almanac_map {
  public:
    Almanac_map(Category from = Category::seed, Category to = Category::soil);

    void add_range(const Item_range<T> &range);
    Item<T> convert(Item<T> in) const;

    bool operator==(const Almanac_map<T> &other) const = default;

    friend std::ostream &operator<< <>(std::ostream &, const Almanac_map<T> &);

    std::pair<Category, Category> categories() const { return {from, to}; };

  private:
    Category from;
    Category to;
    std::set<Item_range<T>> item_ranges{};
};

template <typename T>
std::istream &operator>>(std::istream &in, Almanac_map<T> &almanac_map);