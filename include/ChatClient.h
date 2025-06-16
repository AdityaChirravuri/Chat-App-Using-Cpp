#pragma once
#include <string>

class ChatClient {
public:
    ChatClient(const std::string& username, const std::string& room, int port);
    void start();

private:
    std::string username;
    std::string room;
    int port;
    int sock;

    void receive_messages();
};

