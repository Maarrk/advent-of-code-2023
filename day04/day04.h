#include <istream>
#include <streambuf>
#include <vector>

int read_card_matches(std::istream &in);
int read_header(std::istream &in);
std::vector<int> win_card_counts(const std::vector<int> &matches);

// Template functions need to be fully defined where declared
template <typename S>
inline void read_winning(S &filled_set, std::istream &in) {
    while (in) {
        typename S::key_type number;
        in >> number;
        if (in.good()) {
            filled_set.insert(number);
        } else if (in.fail()) {
            in.clear();
            char letter;
            in >> letter;

            if (letter == '|')
                return;
            else
                throw std::runtime_error{"expected number or '|' character"};
        }
    }

    throw std::runtime_error{"expected '|' before end of input"};
}

template <typename S>
inline int read_winning_count(S &winning_set, std::istream &in) {

    int count{0};
    typename S::key_type number;

    // HACK: read into temporary string and create a new stream
    std::string line;
    std::getline(in, line);
    std::istringstream line_in{line};

    // if passed empty stream good() is true before first read
    while (line_in.peek() != std::istream::traits_type::eof()) {
        line_in >> number;
        if (!line_in.good() && !line_in.eof()) {
            throw std::runtime_error{"expected stream of numbers"};
        }

        if (winning_set.contains(number))
            count++;
    }

    return count;
}
