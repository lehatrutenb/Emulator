#include "gtest/gtest.h" // cmake see, but vs code no

#include <cstdio>
#include <random>
#include <vector>


#include "../src/stack.cpp"


TEST(CheckPushPopTop, 1) {
    myStack::stack<int> s;
    s.push(1);
    EXPECT_EQ(s.top(), 1);
    EXPECT_EQ(s.pop(), 1);
}

TEST(CheckPushPopTop, 2) {
    myStack::stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);

    EXPECT_EQ(s.top(), 3);
    EXPECT_EQ(s.pop(), 3);

    EXPECT_EQ(s.top(), 2);
    EXPECT_EQ(s.pop(), 2);

    EXPECT_EQ(s.top(), 1);
    EXPECT_EQ(s.pop(), 1);
}

TEST(CheckPushCopy, 3) {
    myStack::stack<std::string> s;
    std::string text = "some text";
    s.push(text);
    EXPECT_EQ(s.pop(), text);

    EXPECT_THROW(myStack::stack<int*> s2, std::invalid_argument);
}


TEST(CheckTopPopException, 1) {
    myStack::stack<int> s;

    EXPECT_THROW(s.top(), std::out_of_range);
    EXPECT_THROW(s.pop(), std::out_of_range);

    s.push(1);
    s.pop();

    EXPECT_THROW(s.top(), std::out_of_range);
    EXPECT_THROW(s.pop(), std::out_of_range);
}


TEST(CheckCopy, 1) {
    myStack::stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);

    auto s2(s);
    auto s3 = s;

    for (int i = 3; i >= 1; i--) {
        EXPECT_EQ(s.pop(), i);
    }
    for (int i = 3; i >= 1; i--) {
        EXPECT_EQ(s2.pop(), i);
    }
    for (int i = 3; i >= 1; i--) {
        EXPECT_EQ(s3.pop(), i);
    }
}

TEST(CheckCopy, 2) {
    myStack::stack<int> s;
    s.push(1);
    auto s2(s);
    auto s3 = s;

    s2.push(2);
    s3.push(2);

    s.push(3);

    EXPECT_EQ(s.pop(), 3);
    EXPECT_EQ(s.pop(), 1);

    EXPECT_EQ(s2.pop(), 2);
    EXPECT_EQ(s3.pop(), 2);

    EXPECT_EQ(s2.pop(), 1);
    EXPECT_EQ(s3.pop(), 1);
}

TEST(CheckMove, 1) {
    myStack::stack<int> s;
    s.push(1);
    auto s2(std::move(s));
    EXPECT_EQ(s2.pop(), 1);

    s2.push(2);

    auto s3(std::move(s2));
    EXPECT_EQ(s3.pop(), 2);
}

TEST(CheckMoveRights, 1) {
    myStack::stack<int> s;
    s.push(1);
    auto s2(std::move(s));

    EXPECT_THROW(s.pop(), std::out_of_range);
    EXPECT_THROW(s.top(), std::out_of_range);
}

int main(int argc, char **argv) {
    srand(1303);

    ::testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}