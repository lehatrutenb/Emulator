#include "gtest/gtest.h"

#include <cstdio>
#include <random>
#include <vector>
#include <string>

#include "../src/preprocessor.cpp"

using namespace preprocessor;


TEST(CheckSimplePrograms, 0) {
    std::string inputs[] = {"BEGIN\nEND", "BEGIN END\0"};
    
    Preprocessor preprocessor;
    std::vector<std::variant<std::shared_ptr<commands::Command>, int>> results[] = {preprocessor.preprocess(std::move(inputs[0])), preprocessor.preprocess(std::move(inputs[1]))};
    
    EXPECT_EQ(results[0].size(), 2);
    EXPECT_EQ(results[1].size(), 2);
    
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<commands::Command>>(results[0][0]));
    EXPECT_TRUE(std::holds_alternative<std::shared_ptr<commands::Command>>(results[1][0]));

    for (int i = 0; i < 2; i++) {
        EXPECT_TRUE(std::holds_alternative<std::shared_ptr<commands::Command>>(results[0][i]));
        EXPECT_TRUE(std::holds_alternative<std::shared_ptr<commands::Command>>(results[1][i]));
    }
}

TEST(CheckBadProgram, BeginEnd) {
    std::string inputs[] = {"", "BEGIN", "END", "BEGINEND", "ENDBEGIN"};
    
    Preprocessor preprocessor;
    for (int i = 0; i < 5; i++) {
        EXPECT_THROW(preprocessor.preprocess(std::move(inputs[i]));, std::invalid_argument);
    }
}

TEST(CheckBadProgram, ArgsAmount) {
    std::string inputs[] = {"BEGIN PUSH END", "BEGIN PUSH PUSH 1 END", "BEGIN PUSHR END", "BEGIN POPR END",
                            "BEGIN JMP END", "BEGIN JEQ END", "BEGIN JNE END", "BEGIN JA END", "BEGIN JAE END", "BEGIN JB END", "BEGIN JBE END", "BEGIN CALL END"};

    Preprocessor preprocessor;
    for (int i = 0; i < 12; i++) {
        EXPECT_THROW(preprocessor.preprocess(std::move(inputs[i]));, std::invalid_argument);
    }
}

TEST(CheckLabelIndexing, 0) {
    std::string inputs[] = {"BEGIN label1 label2 label3 label1 END", "BEGIN hahalabel PUSH 1 label2 END"};
    
    Preprocessor preprocessor;
    std::vector<std::variant<std::shared_ptr<commands::Command>, int>> results[] = {preprocessor.preprocess(std::move(inputs[0])), preprocessor.preprocess(std::move(inputs[1]))};
    
    EXPECT_EQ(std::get<int>(results[0][1]), 4);
    EXPECT_EQ(std::get<int>(results[0][2]), 2);
    EXPECT_EQ(std::get<int>(results[0][3]), 3);
    EXPECT_EQ(std::get<int>(results[0][4]), 4);

    EXPECT_EQ(std::get<int>(results[1][1]), 1);
    EXPECT_EQ(std::get<int>(results[1][4]), 4);
}

TEST(CheckReg, Indexing) {
    std::string inputs[] = {"BEGIN reg0 reg1 reg2 reg3 END"};
    
    Preprocessor preprocessor;
    std::vector<std::variant<std::shared_ptr<commands::Command>, int>> results[] = {preprocessor.preprocess(std::move(inputs[0]))};
    
    EXPECT_EQ(std::get<int>(results[0][1]), 0);
    EXPECT_EQ(std::get<int>(results[0][2]), 1);
    EXPECT_EQ(std::get<int>(results[0][3]), 2);
    EXPECT_EQ(std::get<int>(results[0][4]), 3);
}

TEST(CheckReg, BadIndexing) {
    std::string inputs[] = {"BEGIN reg4 END", "BEGIN reg END",  "BEGIN reg-1 END"};
    
    Preprocessor preprocessor;
    std::vector<std::variant<std::shared_ptr<commands::Command>, int>> results[] = {preprocessor.preprocess(std::move(inputs[0])), preprocessor.preprocess(std::move(inputs[1])), preprocessor.preprocess(std::move(inputs[2]))};
    
    EXPECT_EQ(std::get<int>(results[0][1]), 1); // like a label
    EXPECT_EQ(std::get<int>(results[1][1]), 1);
    EXPECT_EQ(std::get<int>(results[2][1]), 1);
}


int main(int argc, char **argv) {
    srand(1303);

    ::testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}