#include "ITape.hpp"
#include "Config.hpp"
#include <fstream>
#include <thread>
#include <chrono>

class Tape : IType {
    private:
        std::fstream data;
        size_t current_position;

        // Задержки
        int rw_delay;
        int rewind_delay;
        int move_delay;

    public:
        Tape(std::string file_name, Config conf);

        std::optional<int32_t> read() override;
        bool write(int32_t value) override;
        bool rewind() override;
        bool move(bool direction) override;

        ~Tape();
};


