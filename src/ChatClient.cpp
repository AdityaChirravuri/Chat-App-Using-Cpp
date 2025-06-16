#include "../include/ChatClient.h"
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <cstring>

ChatClient::ChatClient(const std::string& u, const std::string& r, int p)
    : username(u), room(r), port(p) {}

void ChatClient::receive_messages() {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int len = read(sock, buffer, sizeof(buffer));
        if (len <= 0) break;
        std::cout << std::string(buffer, len);
        if (buffer[len - 1] != '\n') std::cout << std::endl;
    }
}

void ChatClient::start() {
    struct sockaddr_in serv_addr{};
    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection Failed\n";
        return;
    }

    send(sock, username.c_str(), username.length(), 0);
    sleep(1);
    send(sock, room.c_str(), room.length(), 0);

    std::thread listener(&ChatClient::receive_messages, this);
    listener.detach();

    std::string msg;
    while (true) {
        std::getline(std::cin, msg);
        if (msg == "/exit") break;

        // Just send message as is, server will handle private or broadcast
        send(sock, (msg + "\n").c_str(), msg.length() + 1, 0);
    }

    close(sock);
}

