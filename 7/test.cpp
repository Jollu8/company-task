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
    EXPECT_EQ(Server::CalculateSHA1("test"s), "a94a8fe5ccb19ba61c4c0873d391e987982fbbd3"s);
}
TEST_F(ServerTest, CountHits) {
    std::unordered_map<std::string, int> hit_count_Map;
    Server::CountHits("test", hit_count_Map);
    Server::CountHits("test", hit_count_Map);
    EXPECT_EQ(hit_count_Map["test"], 2);
}


TEST_F(ServerTest, ExtractPath) {
    std::string request = "GET /test HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: curl/7.58.0\r\nAccept: */*\r\n\r\n";
    EXPECT_EQ(Server::ExtractPath(request), "/test");
}

TEST_F(ServerTest, ExtractUserAgent) {
    std::string request = "GET /test HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: curl/7.58.0\r\nAccept: */*\r\n\r\n";
    EXPECT_EQ(Server::ExtractUserAgent(request), "curl/7.58.0");
}




int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


