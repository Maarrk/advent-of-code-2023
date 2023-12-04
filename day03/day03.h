#include <istream>
#include <string>

#include <experimental/mdarray>
#include <experimental/mdspan>

namespace stdex = std::experimental;

using char_mat = stdex::mdarray<char, std::dextents<char, 2>>;

char_mat load_istream(std::istream &in);