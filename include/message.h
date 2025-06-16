#pragma once
#include <string>

class Message {
public:
    std::string sender;
    std::string content;

    Message(const std::string& s, const std::string& c)
        : sender(s), content(c) {}

    std::string to_string() const {
        return sender + ": " + content;
    }
};

