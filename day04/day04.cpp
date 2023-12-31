#include <algorithm>
#include <array>
#include <format>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

#include "doctest.h"

#include "day04.h"

using namespace std::literals;

int read_card_matches(std::istream &in) {
    read_header(in);
    std::set<int> winning_numbers{};
    read_winning(winning_numbers, in);
    int count = read_winning_count(winning_numbers, in);
    return count;
}

int read_header(std::istream &in) {
    const std::array<char, 5> word{"Card"};
    std::array<char, word.size()> read_buf{};

    // get reads one less than buffer size, leaving null terminator
    in.get(read_buf.data(), read_buf.size());
    if (in.eof())
        throw std::runtime_error{"input too short"};

    if (word != read_buf)
        throw std::runtime_error{
            std::format("expected 'Card' at start, got '{}'", read_buf.data())};

    int number;
    in >> number;
    if (!in.good())
        throw std::runtime_error{"expected card number"};

    char colon;
    in >> colon;
    if (!in.good() || colon != ':')
        throw std::runtime_error{"expected ':' after card number"};

    return number;
}

std::vector<int> win_card_counts(const std::vector<int> &matches) {
    std::vector<int> counts(matches.size(), 1);

    auto outer_match_it = matches.begin();
    auto outer_count_it = counts.begin();
    while (outer_match_it != matches.end()) {
        int match_count = *outer_match_it;
        int instance_count = *outer_count_it;

        if (match_count > std::distance(outer_count_it, counts.end()))
            throw std::logic_error{std::format(
                "A card had {} matches, while being {}th card from the "
                "end, which would overflow",
                match_count, std::distance(outer_count_it, counts.end()))};

        auto inner_count_it = outer_count_it;
        for (size_t i = 0; i < match_count; i++) {
            *(++inner_count_it) += instance_count;
        }

        outer_match_it++;
        outer_count_it++;
    }

    return counts;
}
