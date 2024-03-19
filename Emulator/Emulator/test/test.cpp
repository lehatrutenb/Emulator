#include "gtest/gtest.h"

#include <cstdio>
#include <random>
#include <vector>
#include <string>

#include "../../Emulator/src/emulator.cpp"

using namespace emulator;


TEST(CheckSmallProgramm, 0) {
    std::ofstream file("input");
    file << "BEGIN PUSH 123 OUT END";
    file.close();

    std::ostringstream output;
    auto cout_buff = std::cout.rdbuf();
    std::cout.rdbuf(output.rdbuf());

    emulator::Emulator emulator("./input");
    emulator.run();

    std::cout.rdbuf(cout_buff);

    EXPECT_EQ(output.str(), "123");
}

TEST(CheckSmallProgramm, 1) {
    std::ofstream file("input");
    file << "BEGIN PUSH 123 POPR reg0 PUSHR reg0 PUSHR reg0 POP OUT END";
    file.close();
    
    std::ostringstream output;
    auto cout_buff = std::cout.rdbuf();
    std::cout.rdbuf(output.rdbuf());

    emulator::Emulator emulator("./input");
    emulator.run();

    std::cout.rdbuf(cout_buff);

    EXPECT_EQ(output.str(), "123");
}

TEST(CheckSmallProgramm, 2) {
    std::ofstream file("input");
    file << "BEGIN PUSH -123 PUSH 2 MUL OUT END";
    file.close();
    
    std::ostringstream output;
    auto cout_buff = std::cout.rdbuf();
    std::cout.rdbuf(output.rdbuf());

    emulator::Emulator emulator("./input");
    emulator.run();

    std::cout.rdbuf(cout_buff);

    std::cout << output.str() << std::endl;

    EXPECT_EQ(output.str(), "-246");
}

TEST(CheckSmallProgramm, 3) {
    std::ofstream file("input");
    file << "BEGIN PUSH 2 PUSH 10 DIV PUSH 100 SUB OUT END";
    file.close();
    
    std::ostringstream output;
    auto cout_buff = std::cout.rdbuf();
    std::cout.rdbuf(output.rdbuf());

    emulator::Emulator emulator("./input");
    emulator.run();

    std::cout.rdbuf(cout_buff);

    std::cout << output.str() << std::endl;

    EXPECT_EQ(output.str(), "95");
}


TEST(CheckIn, 0) {
    std::ofstream file("input");
    file << "BEGIN IN OUT END";
    file.close();

    std::istringstream input;
    input.str("123");
    auto cin_buff = std::cin.rdbuf();
    std::cin.rdbuf(input.rdbuf());
    
    std::ostringstream output;
    auto cout_buff = std::cout.rdbuf();
    std::cout.rdbuf(output.rdbuf());

    emulator::Emulator emulator("./input");
    emulator.run();

    std::cout.rdbuf(cout_buff);
    std::cin.rdbuf(cin_buff);

    std::cout << output.str() << std::endl;

    EXPECT_EQ(output.str(), "123");
}

int main(int argc, char **argv) {
    srand(1303);

    ::testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}
