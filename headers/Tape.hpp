#pragma once
#include "ITape.hpp"
#include "Config.hpp"
#include <fstream>
#include <thread>
#include <chrono>
#include "Logger.hpp"

class Tape : public ITape {
    private:
        std::fstream data;
        size_t current_position;
        std::shared_ptr<Logger> logger;

        // Задержки
        size_t rw_delay;
        size_t rewind_delay;
        size_t move_delay;

    public:
        Tape(std::string file_name, Config conf, std::shared_ptr<Logger> logger = nullptr);

        std::optional<int32_t> read() override;
        bool write(int32_t value) override;
        bool rewind() override;
        bool move(bool direction) override;

        ~Tape();
};


