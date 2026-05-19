#pragma once
#include <string>
#include <format>
#include <fstream>
#include <filesystem>
#include <iostream>

enum class Stage {
    split, 
    merge,
    read,
    write,
    rewind,
    move,
    sort,
    tape
};

std::string cast_of_str(Stage st);

class Event {
    private:
        std::string message;
    public:
        Event(Stage state, const std::string& text);
        const std::string& getMessage() const;
};

class Logger {
    private:
        std::ofstream device;

    public:
        Logger(const std::string& file_name);
        void write(const Event& event);
        ~Logger();

};