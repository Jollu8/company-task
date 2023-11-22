//
// Created by Jollu Emil
//

#include "Server.h"

int main() {
    try {
        Server server(8080, 1024);
        server.Run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
