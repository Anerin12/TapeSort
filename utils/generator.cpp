#include "generator.hpp"

void generate_input(int num, std::string file_name)
{
    {
        std::ofstream clear_file("./input.txt", std::ios::trunc);
        if (!clear_file.is_open())
            return;
    }

    Tape input_tape(file_name, {0, 0, 0, 0});

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int32_t> distr(INT32_MIN, INT32_MAX);

    for (int i = 0; i < num; i++)
    {
        input_tape.write(distr(gen));
        input_tape.move(true);
    }
}