#include "../include/ChatServer.h"
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <algorithm>
#include <cstring>

ChatServer::ChatServer(int port) : port(port) {}

void ChatServer::broadcast(const std::string& room, const Message& msg, int exclude_fd) {
    std::lock_guard<std::mutex> lock(room_mutex);
    if (rooms.count(room)) {
        std::string text = msg.to_string();
        for (const auto& client : rooms[room]) {
            if (client.fd != exclude_fd) {
                send(client.fd, text.c_str(), text.size(), 0);
            }
        }
    }
}

void ChatServer::print_active_rooms() {
    std::lock_guard<std::mutex> lock(room_mutex);
    std::cout << "\n=== Active Rooms ===\n";
    if (rooms.empty()) {
        std::cout << "No active rooms.\n";
    } else {
        for (const auto& pair : rooms) {
            std::cout << " - " << pair.first << " (" << pair.second.size() << " users)\n";
        }
    }
    std::cout << "====================\n\n";
}

void ChatServer::handle_client(int client_fd) {
    char buffer[1024] = {0};

    // Read username
    int len = read(client_fd, buffer, sizeof(buffer));
    if (len <= 0) {
        close(client_fd);
        return;
    }
    std::string username(buffer, len);

    // Read room
    memset(buffer, 0, sizeof(buffer));
    len = read(client_fd, buffer, sizeof(buffer));
    if (len <= 0) {
        close(client_fd);
        return;
    }
    std::string room(buffer, len);

    {
        std::lock_guard<std::mutex> lock(room_mutex);
        rooms[room].push_back({client_fd, username});
    }
    print_active_rooms();

    broadcast(room, Message("Server", username + " joined " + room + "\n"), client_fd);

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int n = read(client_fd, buffer, sizeof(buffer));
        if (n <= 0) break;

        std::string msg(buffer, n);

        // Private message detection
        if (!msg.empty() && msg[0] == '@') {
            size_t space_pos = msg.find(' ');
            if (space_pos != std::string::npos) {
                std::string target_user = msg.substr(1, space_pos - 1);
                std::string private_msg = msg.substr(space_pos + 1);

                bool found = false;
                {
                    std::lock_guard<std::mutex> lock(room_mutex);
                    auto& clients = rooms[room];
                    for (const auto& client : clients) {
                        if (client.username == target_user) {
                            std::string formatted = "(private) " + username + ": " + private_msg;
                            send(client.fd, formatted.c_str(), formatted.size(), 0);
                            found = true;
                            break;
                        }
                    }
                }
                if (!found) {
                    std::string err = "Server: User '" + target_user + "' not found in room.\n";
                    send(client_fd, err.c_str(), err.size(), 0);
                }
            } else {
                std::string err = "Server: Invalid private message format. Use @username <message>\n";
                send(client_fd, err.c_str(), err.size(), 0);
            }
        } else {
            // Broadcast message to room
            broadcast(room, Message(username, msg), client_fd);
        }
    }

    {
        std::lock_guard<std::mutex> lock(room_mutex);
        auto& clients = rooms[room];
        clients.erase(std::remove_if(clients.begin(), clients.end(),
            [client_fd](const Client& c) { return c.fd == client_fd; }), clients.end());
        if (clients.empty()) {
            rooms.erase(room);
        }
    }
    print_active_rooms();

    close(client_fd);
    broadcast(room, Message("Server", username + " left the room.\n"));
}

void ChatServer::start() {
    struct sockaddr_in address{};
    int opt = 1;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 10);

    std::cout << "Broker running on port " << port << "\n";

    while (true) {
        int client_fd = accept(server_fd, nullptr, nullptr);
        std::thread(&ChatServer::handle_client, this, client_fd).detach();
    }
}

