//
// Created by Jollu Emil
//

#include "Server.h"

int main() {
    threadpool::Threadpool pool(5);
    std::unordered_map<std::string, int> hit_count_Map;
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Error creating socket." << std::endl;
        return -1;
    }

    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    if (bind(server_socket, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address)) == -1) {
        std::cerr << "Error binding socket." << std::endl;
        close(server_socket);
        return 1;
    }

    if (listen(server_socket, 10) == -1) {
        std::cerr << "Error listening on socket." << std::endl;
        close(server_socket);
        return 1;
    }

    std::cout << "Server listening on port: " << PORT << "..." << std::endl;

    while (true) {
        sockaddr_in client_address{};
        socklen_t client_address_len = sizeof(client_address);
        int client_socket = accept(server_socket, reinterpret_cast<sockaddr*>(&client_address), &client_address_len);

        if (client_socket == -1) {
            std::cerr << "Error accepting connection. " << std::endl;
            continue;
        }

        ProcessRequest(client_socket, pool, hit_count_Map);
    }

    close(server_socket);
}