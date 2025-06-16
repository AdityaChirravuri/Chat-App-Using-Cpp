#include <iostream>
#include "include/ChatServer.h"
#include "include/ChatClient.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage:\n"
                  << "  ./chat_app broker [port]\n"
                  << "  ./chat_app client <username> <room> [port]\n";
        return 1;
    }

    std::string mode = argv[1];
    int port = 9090;  // default port

    if (mode == "broker") {
        if (argc >= 3) port = std::stoi(argv[2]);
        ChatServer server(port);
        server.start();
    }
    else if (mode == "client") {
        if (argc < 4) {
            std::cerr << "Client mode requires: <username> <room> [port]\n";
            return 1;
        }
        std::string username = argv[2];
        std::string room = argv[3];
        if (argc >= 5) port = std::stoi(argv[4]);
        ChatClient client(username, room, port);
        client.start();
    }
    else {
        std::cerr << "Invalid mode. Use 'broker' or 'client'.\n";
        return 1;
    }

    return 0;
}

