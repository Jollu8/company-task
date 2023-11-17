//
/// Created by Jollu Emil
//

#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "Server.h"

TEST(CalculateSHA1Test, ReturnsCorrectSHA1) {
    std::string input = "Hello, world!";
    std::string expected_output = "943a702d06f34599aee1f8da8ef9f7296031d699";
    EXPECT_EQ(CalculateSHA1(input), expected_output);

    std::vector<std::string> output;
    output.reserve(10);

    for (auto i = 1; i < 11; ++i) {
        output.emplace_back(CalculateSHA1(std::to_string(i)));
    }

    for (auto i = 0; i < 10; ++i) {
        EXPECT_EQ(CalculateSHA1(std::to_string(i + 1)), output[i]);
    }
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}