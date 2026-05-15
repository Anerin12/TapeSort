#include "Tape.hpp"

const int ENTRY_SIZE = 12;

Tape::Tape(std::string file_name, Config conf) : rw_delay(conf.rw_delay),
                                                rewind_delay(conf.rewind_delay),
                                                move_delay(conf.move_delay) 
{
    // Открываем поток ввода (ленту)
    this->data.open(file_name, std::ios::in | std::ios::out);

    if (!this->data.is_open())
    {
        this->data.clear();
        this->data.open(file_name, std::ios::in | std::ios::out | std::ios::trunc);
    }

    if (!this->data.is_open())
    {
        perror(("Failed open file: " + file_name).c_str());
        exit(1);
    }

    // Отключю буферизация потоков для минимизации потребления RAM и более точной эмуляции последовательного доступа ленты
    this->data.rdbuf()->pubsetbuf(nullptr, 0);

    this->current_position = 0;
}

Tape::~Tape(){
    this->data.close();
}

std::optional<int32_t> Tape::read(){

    int32_t buffer = 0;
    std::streampos bytePosition = static_cast<std::streamoff>(this->current_position) * ENTRY_SIZE;

    this->data.seekg(bytePosition);

    if (this->data >> buffer)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(rw_delay));
        return buffer;
    }

    return std::nullopt;
}

bool Tape::write(int32_t value) {
    std::streampos bytePosition = static_cast<std::streamoff>(this->current_position) * ENTRY_SIZE;
    this->data.seekp(bytePosition);

    if (this->data << std::setw(ENTRY_SIZE - 1) << std::setfill(' ') << value << " ")
    {
        this->data.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(rw_delay));
        return true;
    }

    return false;
}

bool Tape::rewind() {
    this->current_position = 0;
    std::streampos bytePosition = static_cast<std::streamoff>(this->current_position) * sizeof(int32_t);

    this->data.seekg(bytePosition);
    std::this_thread::sleep_for(std::chrono::milliseconds(this->rewind_delay));

    return true;
}

bool Tape::move(bool direction) {
    if (direction) {
        this->current_position++;
    } 
    else {
        if (current_position == 0) return false;
        this->current_position--;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(move_delay));

    return true;
}