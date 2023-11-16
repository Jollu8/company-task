//
/// Created by Jollu Emil
//
#include "Server.h"
#include "gtest/gtest.h"


TEST(ServerTest, HandlesTasksCorrectly) {
    Server server;

    std::thread serverThread([&server]{ server.run(); });

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in servaddr{};
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(8080);
    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    std::string request = "GET /path HTTP/1.1\r\nUser-Agent: TestAgent\r\n\r\n";
    write(sockfd, request.c_str(), request.size());


    std::this_thread::sleep_for(std::chrono::seconds(3));

    EXPECT_EQ(server.getPathHitCount("/path"), 1);
    EXPECT_EQ(server.getUserAgentHitCount("TestAgent"), 1);

    close(sockfd);
    serverThread.join();
}

