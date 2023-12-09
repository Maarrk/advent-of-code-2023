#include <algorithm>
#include <array>
#include <chrono>
#include <format>
#include <iostream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "doctest.h"

#include "day05.h"

using namespace std::literals;

static const std::map<Category, const char *> category_names{
    {Category::seed, "seed"},
    {Category::soil, "soil"},
    {Category::fertilizer, "fertilizer"},
    {Category::water, "water"},
    {Category::light, "light"},
    {Category::temperature, "temperature"},
    {Category::humidity, "humidity"},
    {Category::location, "location"},
};

std::pair<num, num> solution(std::istream &input_file, bool verbose) {
    std::string line{};

    std::vector<num> initial_numbers;
    {
        std::getline(input_file, line);
        std::istringstream first_line{line};

        std::string skipped_word{};
        first_line >> skipped_word;

        num number;
        first_line >> number;
        while (!first_line.fail()) {
            initial_numbers.push_back(number);
            first_line >> number;
        }

        std::getline(input_file, line); // skip next empty line
    }

    std::vector<Item<num>> items;
    std::for_each(initial_numbers.begin(), initial_numbers.end(), [&](num n) {
        items.push_back({Category::seed, n});
    });

    if (verbose) {
        std::cout << "Initial: ";
        for (auto &&i : items) {
            std::cout << i << ", ";
        }
        std::cout << "\n\n";
    }

    std::array<Almanac_map<num>, 7> almanac;
    for (Almanac_map<num> &am : almanac) {
        input_file >> am;
    }

    if (verbose) {
        for (const auto &am : almanac) {
            std::cout << am;
        }
    }

    std::vector<num> output;
    output.reserve(items.size());
    for (auto &item : items) {
        if (verbose) {
            std::cout << "- " << item << ", ";
        }
        for (const auto &am : almanac) {
            item = am.convert(item);
            if (verbose) {
                std::cout << item << ", ";
            }
        }
        output.push_back(item.number);
        if (verbose) {
            std::cout << std::endl;
        }
    }
    if (verbose) {
        std::cout << std::endl;
    }

    num part1 = *std::min_element(output.begin(), output.end());

    // HACK: brute force (unfeasible)
    auto start_time = std::chrono::steady_clock::now();

    items.clear();
    for (size_t i = 0; i < initial_numbers.size(); i += 2) {
        num start = initial_numbers[i];
        num count = initial_numbers[i + 1];
        std::cout << "Creating " << count << " items from " << start << "\t"
                  << static_cast<double>(i) /
                         static_cast<double>(initial_numbers.size())
                  << std::endl;
        for (num j = 0; j < count; j++) {
            items.push_back({Category::seed, start + j});
        }
    }

    output.clear();
    output.reserve(items.size());

    auto last_print_time = start_time;
    for (auto &item : items) {
        for (const auto &am : almanac) {
            item = am.convert(item);
        }
        output.push_back(item.number);

        auto time = std::chrono::steady_clock::now();
        if (time - last_print_time >= 10s || true) {
            std::cout << output.size() << "/" << items.size() << "\t"
                      << static_cast<double>(output.size()) /
                             static_cast<double>(items.size())
                      << std::endl;
            last_print_time = time;
        }
    }

    num part2 = *std::min_element(output.begin(), output.end());

    return {part1, part2};
}

std::ostream &operator<<(std::ostream &out, const Category &category) {
    return out << category_names.find(category)->second;
}

int operator-(const Category &lhs, const Category &rhs) {
    return static_cast<int>(lhs) - static_cast<int>(rhs);
}

template <typename T>
std::ostream &operator<<(std::ostream &out, const Item<T> &item) {
    return out << item.category << " " << item.number;
}

template <typename T>
std::istream &operator>>(std::istream &in, Item_range<T> &item_range) {
    T dst, src, len;
    in >> dst >> src >> len;
    item_range = Item_range{dst, src, len};
    return in;
}

// Maybe this should follow the raw serialisation like Almanac_map, and be
// pretty-printable with format?
template <typename T>
std::ostream &operator<<(std::ostream &out, const Item_range<T> &item_range) {
    return out << "Item_range{" << item_range.destination_start << ", "
               << item_range.source_start << ", " << item_range.length << "}";
}

TEST_CASE("reading item ranges") {
    std::istringstream in{
        R"(50 98 2
52 50 48
3305253869 1699909104 39566623
3344820492 1130725752 384459310

)"};
    Item_range<int> range;
    in >> range;
    CHECK_EQ(range, Item_range<int>{50, 98, 2});
    in >> range;
    CHECK_EQ(range, Item_range<int>{52, 50, 48});

    using Long_range = Item_range<unsigned long>;

    Long_range long_range;
    in >> long_range;
    CHECK_EQ(long_range, Long_range{3305253869, 1699909104, 39566623});
    in >> long_range;
    CHECK_EQ(long_range, Long_range{3344820492, 1130725752, 384459310});

    in >> long_range;
    CHECK_FALSE(in.good());
    CHECK(in.fail());
}

template <typename T>
Almanac_map<T>::Almanac_map(Category from, Category to) : from{from}, to{to} {
    if (to - from != 1) {
        throw std::invalid_argument{
            "Every almanac map must convert to the next category"};
    }
}

TEST_CASE("almanac constructor") {
    using C = Category;

    CHECK_NOTHROW(Almanac_map<int>(C::seed, C::soil));
    CHECK_NOTHROW(Almanac_map<int>(C::soil, C::fertilizer));
    CHECK_THROWS(Almanac_map<int>(C::water, C::water));
    CHECK_THROWS(Almanac_map<int>(C::water, C::fertilizer));
    CHECK_THROWS(Almanac_map<int>(C::water, C::temperature));
}

template <typename T>
void Almanac_map<T>::add_range(const Item_range<T> &range) {
    item_ranges.insert(range);
}

template <typename T> Item<T> Almanac_map<T>::convert(Item<T> in) const {
    if (in.category != from) {
        throw std::invalid_argument{"incorrect item category"};
    }

    T number = in.number;
    auto range_it = std::find_if(
        item_ranges.begin(), item_ranges.end(), [number](Item_range<T> range) {
            return number >= range.source_start &&
                   number < range.source_start + range.length;
        });
    if (range_it != item_ranges.end()) {
        number += range_it->destination_start - range_it->source_start;
    }

    return {to, number};
}

TEST_CASE("example seed-to-soil") {
    Almanac_map<int> am{Category::seed, Category::soil};
    am.add_range({50, 98, 2});
    am.add_range({52, 50, 48});
    const auto item = [](int number) {
        return Item<int>{Category::seed, number};
    };

    SUBCASE("unmapped") {
        auto out = am.convert(item(10));
        CHECK_EQ(out.category, Category::soil);
        CHECK_EQ(out.number, 10);
    }
    SUBCASE("example list") {
        CHECK_EQ(am.convert(item(0)).number, 0);
        CHECK_EQ(am.convert(item(1)).number, 1);

        CHECK_EQ(am.convert(item(48)).number, 48);
        CHECK_EQ(am.convert(item(49)).number, 49);
        CHECK_EQ(am.convert(item(50)).number, 52);
        CHECK_EQ(am.convert(item(51)).number, 53);

        CHECK_EQ(am.convert(item(96)).number, 98);
        CHECK_EQ(am.convert(item(97)).number, 99);
        CHECK_EQ(am.convert(item(98)).number, 50);
        CHECK_EQ(am.convert(item(99)).number, 51);
    }
    SUBCASE("example input") {
        CHECK_EQ(am.convert(item(79)).number, 81);
        CHECK_EQ(am.convert(item(14)).number, 14);
        CHECK_EQ(am.convert(item(55)).number, 57);
        CHECK_EQ(am.convert(item(13)).number, 13);
    }
}

template <typename T>
std::istream &operator>>(std::istream &in, Almanac_map<T> &almanac_map) {
    std::istream::pos_type start_pos = in.tellg();
    try {
        std::string line;
        std::getline(in, line);
        size_t separator = line.find("-to-");
        size_t suffix = line.find(" map:");
        if (separator == line.npos || suffix == line.npos ||
            suffix != line.length() - 5) {
            throw std::invalid_argument{"couldn't separate header string"};
        }

        std::string_view from_name{line.data(), separator};
        std::string_view to_name{&line[separator + 4],
                                 suffix - (separator + 4)};
        auto from_it = std::find_if(
            category_names.begin(), category_names.end(),
            [from_name](auto kv) { return from_name == kv.second; });
        auto to_it =
            std::find_if(category_names.begin(), category_names.end(),
                         [to_name](auto kv) { return to_name == kv.second; });
        if (from_it == category_names.end() || to_it == category_names.end()) {
            throw std::invalid_argument{"unrecognised category"};
        }
        Almanac_map<T> am{from_it->first, to_it->first};

        while (in.good()) {
            Item_range<T> ir;
            in >> ir;
            if (!in.fail()) {
                am.add_range(ir);
            }
        }
        // clear only failbit
        in.clear((in.rdstate() & in.badbit) | (in.rdstate() & in.eofbit));

        almanac_map = am;
    } catch (const std::invalid_argument &e) {
        in.seekg(start_pos);
        in.setstate(in.failbit);
    }
    return in;
}

template <typename T>
std::ostream &operator<<(std::ostream &out, const Almanac_map<T> &almanac_map) {
    out << almanac_map.from << "-to-" << almanac_map.to << " map:" << std::endl;
    for (Item_range<T> ir : almanac_map.item_ranges) {
        out << ir.destination_start << " " << ir.source_start << " "
            << ir.length << std::endl;
    }
    return out;
}

TEST_CASE("(de)serialising almanac map") {
    {
        std::stringstream buf{};
        Almanac_map<int> am1(Category::fertilizer, Category::water);
        buf << am1;
        Almanac_map<int> am2;
        buf >> am2;
        CHECK_EQ(am1, am2);
    }
    {
        std::istringstream in{
            "seed-to-soil map:\n50 98 2\n52 50 48\n\nother data"};
        Almanac_map<int> am1;
        in >> am1;
        CHECK_EQ(in.rdstate(), in.goodbit);

        // Should be order-independent
        Almanac_map<int> am2;
        am2.add_range({52, 50, 48});
        am2.add_range({50, 98, 2});

        CHECK_EQ(am1, am2);

        std::string remaining;
        in >> remaining;
        CHECK_EQ(remaining, "other"s);
        in >> remaining;
        CHECK_EQ(remaining, "data"s);
    }
    {
        std::istringstream in{
            R"(seed-to-soil map:
50 98 2
52 50 48

soil-to-fertilizer map:
0 15 37
37 52 2
39 0 15
)"};
        Almanac_map<int> am1, am2;
        in >> am1 >> am2;

        CHECK_EQ(am1.categories().first, Category::seed);
        CHECK_EQ(am1.categories().second, Category::soil);
        CHECK_EQ(am2.categories().first, Category::soil);
        CHECK_EQ(am2.categories().second, Category::fertilizer);
    }
}