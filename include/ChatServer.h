#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include "message.h"

class ChatServer {
public:
    ChatServer(int port);
    void start();

private:
    int server_fd;
    int port;

    struct Client {
        int fd;
        std::string username;
    };

    std::unordered_map<std::string, std::vector<Client>> rooms;
    std::mutex room_mutex;

    void handle_client(int client_fd);
    void broadcast(const std::string& room, const Message& msg, int exclude_fd = -1);
    void print_active_rooms();  // Added function to print active rooms on server console
};

