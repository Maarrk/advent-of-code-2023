#include <istream>
#include <string>

#include <experimental/mdspan>

namespace stdex = std::experimental;

struct load_istream_result {
    std::string_view text;
    stdex::mdspan<char, std::dextents<char, 2>> mds;
};
load_istream_result load_istream(std::istream &in, std::string &container);