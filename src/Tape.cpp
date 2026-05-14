#include "./headers/Tape.hpp"

Tape::Tape(std::string file_name) : data(file_name, std::ios::in | std::ios::out | std::ios::binary)
{

    // Получение задержек из файла
    std::fstream config("../config/config.txt");

    this->rw_delay = 0;
    this->rewind_delay = 0;
    this->move_delay = 0;

    // Открываем поток ввода (ленту)
    if (!this->data.is_open()){
        perror("Failed open file");
        exit(1);
    }

    this->current_position = 0;
}

Tape::~Tape(){
    this->data.close();
}

std::optional<int32_t> Tape::read(){

    int32_t buffer = 0;
    std::streampos bytePosition = static_cast<std::streamoff>(this->current_position) * sizeof(int32_t);

    this->data.seekg(bytePosition);

    if (this->data.read(reinterpret_cast<char *>(&buffer), sizeof(int32_t))) {
        std::this_thread::sleep_for(std::chrono::milliseconds(rw_delay));

        return buffer;
    }

    return std::nullopt;
}

bool Tape::write(int32_t value) {
    std::streampos bytePosition = static_cast<std::streamoff>(this->current_position) * sizeof(int32_t);
    this->data.seekp(bytePosition);
    if (this->data.write(reinterpret_cast<char *>(&value), sizeof(int32_t))) {
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