//
/// Created by Jollu Emil
//

#include <gtest/gtest.h>

#include <string>

#include "Server.h"


class ServerTest : public ::testing::Test {
    protected:

};

TEST_F(ServerTest, CalculateSHA1) {
    using namespace std::string_literals;
    EXPECT_EQ(Server::("test"s), "a94a8fe5ccb19ba61c4c0873d391e987982fbbd3"s);
}




int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


