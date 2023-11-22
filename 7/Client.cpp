//
// Created by Jollu Emil


#include <arpa/inet.h>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

#define PORT 8080

int main() {
    sockaddr_in address{};
    int sock = 0;
    sockaddr_in serv_addr{};
    std::string message = "GET / HTTP/1.1\r\n\r\n";
    [[maybe_unused]]std::string message_POST = "POST / HTTP/1.1\r\n\r\n"; //  it does wrong
    char buffer[1024] = {0};

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "\nInvalid address/ Address not supported \n";
        return -1;
    }

    for (int i = 0; i < 5; i++) {
        sleep(1);
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            std::cerr << "\n Socket creation error \n";
            return -1;
        }

        if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
            std::cerr << "\nConnection Failed \n";
            return -1;
        }

        // Split the message into parts and send each part separately
        for (size_t j = 0; j < message.size(); j += 10) {
            std::string part = message.substr(j, 10);
            send(sock, part.c_str(), part.size(), 0);
        }

        std::cout << "GET request sent\n";
        if (read(sock, buffer, 1024) != -1)
            std::cout << buffer << std::endl;

        close(sock);  // Close the socket after each request
    }


    // wong client
    {
        std::cout << "=================Send Post=================" <<std::endl;
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            std::cerr << "\n Socket creation error \n";
            return -1;
        }
        if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
            std::cerr << "\nConnection Failed \n";
            return -1;
        }

        // Split the message into parts and send each part separately
        for (size_t j = 0; j < message_POST.size(); j += 10) {
            std::string part = message_POST.substr(j, 10);
            send(sock, part.c_str(), part.size(), 0);
        }

        std::cout << "GET request sent\n";
        if (read(sock, buffer, 1024) != -1)
            std::cout << buffer << std::endl;

        close(sock);  // Close the socket after each request
    }



    return 0;
}


