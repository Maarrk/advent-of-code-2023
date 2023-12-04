#include <functional>
#include <istream>
#include <string>
#include <vector>

#include <experimental/mdarray>
#include <experimental/mdspan>

namespace stdex = std::experimental;

using char_mat = stdex::mdarray<char, std::dextents<size_t, 2>>;
using mat_it = std::vector<char_mat::element_type>::const_iterator;
using mat_part = std::pair<char_mat::index_type, char_mat::index_type>;

char_mat load_istream(std::istream &in);

mat_it row_end(const char_mat &mat, char_mat::index_type row);
mat_it it_at(const char_mat &mat, char_mat::index_type row,
             char_mat::index_type column);
void iterate_adjacent(
    const char_mat &mat, mat_part rows, mat_part columns,
    std::function<bool(char_mat::element_type)> continue_predicate);

bool is_symbol(char c);