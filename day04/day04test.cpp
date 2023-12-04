#include <sstream>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "day04.h"

TEST_CASE("stream experiments") {
    {
        std::istringstream in{"10 20"};
        int num;

        in >> num;
        CHECK_EQ(num, 10);
        CHECK(in.good());
        CHECK_FALSE(in.eof());
        CHECK_FALSE(in.fail());
        CHECK_FALSE(in.bad());

        in >> num;
        CHECK_EQ(num, 20);
        CHECK_FALSE(in.good());
        CHECK(in.eof());
        CHECK_FALSE(in.fail());
        CHECK_FALSE(in.bad());

        in >> num;
        CHECK_EQ(num, 20);
        CHECK_FALSE(in.good());
        CHECK(in.eof());
        CHECK(in.fail());
        CHECK_FALSE(in.bad());
    }
    {
        std::istringstream in{"10 x 20"};
        int num;
        char let;

        in >> num;
        CHECK_EQ(num, 10);
        CHECK(in.good());
        CHECK_FALSE(in.eof());
        CHECK_FALSE(in.fail());
        CHECK_FALSE(in.bad());

        in >> num;
        CHECK_EQ(num, 0);
        CHECK_FALSE(in.good());
        CHECK_FALSE(in.eof());
        CHECK(in.fail());
        CHECK_FALSE(in.bad());
        CHECK_EQ(in.peek(), -1);

        in.clear();
        in >> let;
        CHECK_EQ(let, 'x');
        CHECK(in.good());
        CHECK_FALSE(in.eof());
        CHECK_FALSE(in.fail());
        CHECK_FALSE(in.bad());
    }
    {
        std::istringstream in{"10 20\n30 40"};
        int num;

        in >> num;
        CHECK_EQ(num, 10);
        CHECK(in.good());

        in >> num;
        CHECK_EQ(num, 20);
        CHECK(in.good());

        // newline is treated like any other whitespace
        in >> num;
        CHECK_EQ(num, 30);
        CHECK(in.good());
    }
    {
        std::istringstream in{""};
        CHECK(in.good());
        CHECK_FALSE(in.eof());
        CHECK_FALSE(in.fail());
        CHECK_FALSE(in.bad());
        CHECK_EQ(in.peek(), -1);
    }
}

TEST_CASE("read_header()") {
    {
        std::istringstream in{""};
        CHECK_THROWS(read_header(in));
    }
    {
        std::istringstream in{"Ca"};
        CHECK_THROWS(read_header(in));
    }
    {
        std::istringstream in{"Game 1: "};
        CHECK_THROWS(read_header(in));
    }
    {
        std::istringstream in{"Ca 1: "};
        CHECK_THROWS(read_header(in));
    }
    {
        std::istringstream in{"Card : "};
        CHECK_THROWS(read_header(in));
    }
    {
        std::istringstream in{"Card 1 "};
        CHECK_THROWS(read_header(in));
    }
    {
        std::istringstream in{"Card 1: "};
        CHECK_EQ(read_header(in), 1);
    }
    {
        std::istringstream in{"Card   123: "};
        CHECK_EQ(read_header(in), 123);
    }
}

TEST_CASE("read_winning()") {
    using set_t = std::unordered_set<int>;
    {
        std::istringstream in{""};
        set_t set{};
        CHECK_THROWS(read_winning(set, in));
    }
    {
        std::istringstream in{"15 |"};
        set_t set{};
        read_winning(set, in);
        CHECK_EQ(set, set_t{15});
    }
    {
        std::istringstream in{"15"};
        set_t set{};
        CHECK_THROWS(read_winning(set, in));
    }
    {
        std::istringstream in{"15 2 |"};
        set_t set{};
        read_winning(set, in);
        CHECK_EQ(set, set_t{2, 15});
    }
}

TEST_CASE("read_winning_count()") {
    using set_t = std::set<int>;
    set_t set{1, 10, 100};
    std::istringstream in{"1 2 3\n10 20  30"};

    SUBCASE("empty") {
        set_t set2{};
        CHECK_EQ(read_winning_count(set2, in), 0);
        std::istringstream in2{""};
        CHECK_EQ(read_winning_count(set, in2), 0);
    }

    SUBCASE("single line") {
        std::istringstream in2{"1 2 3"};
        CHECK_EQ(read_winning_count(set, in2), 1);

        // get one line of multiline string
        CHECK_EQ(read_winning_count(set, in), 1);
    }
}

TEST_CASE("integration") {
    using set_t = std::unordered_set<int>;

    std::istringstream example{
        R"(Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19
Card 3:  1 21 53 59 44 | 69 82 63 72 16 21 14  1
Card 4: 41 92 73 84 69 | 59 84 76 51 58  5 54 83
Card 5: 87 83 26 28 32 | 88 30 70 12 93 22 82 36
Card 6: 31 18 13 56 72 | 74 77 10 23 35 67 36 11)"};

    CHECK_EQ(read_card_value(example), 8); // Card 1
    CHECK_EQ(read_card_value(example), 2); // Card 2
    // CHECK_EQ(read_card_value(example), 2); // Card 3
    {
        CHECK_EQ(read_header(example), 3);
        set_t win_num{};
        read_winning(win_num, example);
        CHECK_EQ(win_num, set_t{1, 21, 53, 59, 44});
        CHECK_EQ(read_winning_count(win_num, example), 2);
    }
    CHECK_EQ(read_card_value(example), 1); // Card 4
    CHECK_EQ(read_card_value(example), 0); // Card 5
    CHECK_EQ(read_card_value(example), 0); // Card 6
    CHECK(example.eof());

    {
        set_t win_num{1, 21, 53, 59, 44};
        std::istringstream card3num{"69 82 63 72 16 21 14  1"};
        CHECK_EQ(read_winning_count(win_num, card3num), 2);
    }
    {
        set_t win_num{1};
        std::istringstream card3num{"69 82 63 72 16 21 14  1"};
        CHECK_EQ(read_winning_count(win_num, card3num), 1);
    }
    {
        set_t win_num{69, 82, 63, 72, 16, 21, 14, 1};
        std::istringstream card3num{"69 82 63 72 16 21 14  1"};
        CHECK_EQ(read_winning_count(win_num, card3num), 8);
    }
}