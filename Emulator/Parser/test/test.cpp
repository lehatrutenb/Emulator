#include "gtest/gtest.h" // cmake see, but vs code no

#include <cstdio>
#include <random>
#include <vector>
#include <string>


#include "../src/parser.cpp"

using namespace tokenparser;


TEST(CheckValsRandomString, 0) {
    std::string input = "BEGIN\nPUSH   SDFSDFDS ADD IN\nEND";
    std::string correct[] = {"BEGIN", "\n", "PUSH", " ", " ", " ", "SDFSDFDS", " ", "ADD", " ", "IN", "\n", "END"};
    auto parser = TokenParser(input);

    for (int i = 0; i < 13; i++) {
        EXPECT_EQ(parser.parse().val, correct[i]);
    }
}

TEST(CheckTypesRandomString, 0) {
    std::string input = "BEGIN\nPUSH   SDFSDFDS ADD IN\nEND";
    TokenType correct[] = {TokenType::WORD, TokenType::DELIMITER, TokenType::WORD, TokenType::DELIMITER, TokenType::DELIMITER,
                           TokenType::DELIMITER, TokenType::WORD, TokenType::DELIMITER, TokenType::WORD, TokenType::DELIMITER,
                           TokenType::WORD, TokenType::DELIMITER, TokenType::WORD, TokenType::END_OF_FILE};

    auto parser = TokenParser(input);

    for (int i = 0; i < 13; i++) {
        EXPECT_EQ(parser.parse().type, correct[i]);
    }
}

int main(int argc, char **argv) {
    srand(1303);

    ::testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}