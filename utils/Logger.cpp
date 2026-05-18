#include "Logger.hpp"

std::string cast_of_str(Stage st)
{
    switch (st)
    {
    case Stage::split:
        return "SPLIT";
    case Stage::merge:
        return "MERGE";
    case Stage::read:
        return "READ";
    case Stage::write:
        return "WRITE";
    case Stage::rewind:
        return "REWIND";
    case Stage::move:
        return "MOVE";
    case Stage::sort:
        return "SORT";
    case Stage::tape:
        return "TAPE";
    default:
        return "UNKNOWN";
    }
}

Event::Event(Stage state, const std::string& text)
{
    message = std::format("[{}] {}", cast_of_str(state), text);
}

const std::string& Event::getMessage() const
{
    return message;
}

Logger::Logger(const std::string& file_name){
    device.open(file_name, std::ios::out);

    if (!device.is_open()){
        std::cerr << std::format("Error: file for logging ({}) can't opened.", file_name) << std::endl;
    }
}

void Logger::write(const Event& event){
    if (device.is_open()){
        device << event.getMessage() << '\n';
    }
}

Logger::~Logger(){
    if (device.is_open()){
        device.close();
    }
}