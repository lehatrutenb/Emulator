#include "gtest/gtest.h" // cmake see, but vs code no

#include <cstdio>
#include <random>
#include <vector>
#include <string>

#include "../src/preprocessor.cpp"

using namespace preprocessor;


TEST(CheckSimplePrograms, 0) {
    std::string inputs[] = {"BEGIN\nEND", "BEGIN END\0"};
    
    Preprocessor preprocessor;
    program_t results[] = {preprocessor.preprocess(std::move(inputs[0])), preprocessor.preprocess(std::move(inputs[1]))};
    
    EXPECT_EQ(results[0].size(), 2);
    EXPECT_EQ(results[1].size(), 2);
    
    EXPECT_TRUE(std::holds_alternative<commands::Command*>(results[0][0]));
    EXPECT_TRUE(std::holds_alternative<commands::Command*>(results[1][0]));

    for (int i = 0; i < 2; i++) {
        EXPECT_TRUE(std::holds_alternative<commands::Command*>(results[0][i]));
        EXPECT_TRUE(std::holds_alternative<commands::Command*>(results[1][i]));
    }
}

int main(int argc, char **argv) {
    srand(1303);

    ::testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}