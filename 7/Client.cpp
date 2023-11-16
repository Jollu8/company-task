//
// Created by Jollu Emil
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

int main() {
    int sock{};
    struct sockaddr_in serv_addr{};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        std::cout << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "Connection Failed" << std::endl;
        return -1;
    }

    const char *message = "GET / HTTP/1.1\r\nUser-Agent: TestClient\r\n\r\n";
    send(sock, message, strlen(message), 0);
    std::cout << "GET request sent" << std::endl;

    char buffer[1024] = {0};
    sleep(1);
    read(sock, buffer, 1024);
    std::cout << buffer << std::endl;

    return 0;
}
//
