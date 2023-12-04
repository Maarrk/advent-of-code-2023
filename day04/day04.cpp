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

int read_card_value(std::istream &in) {
    read_header(in);
    std::set<int> winning_numbers{};
    read_winning(winning_numbers, in);
    int count = read_winning_count(winning_numbers, in);
    if (count == 0) {
        return 0;
    } else {
        return 1 << (count - 1);
    }
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
