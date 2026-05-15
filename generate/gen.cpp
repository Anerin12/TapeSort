#include <random>
#include <fstream>


int main() {
    std::fstream out("./input.txt");

    for (int i = 0; i < 2000; i++){

        std::random_device rd;

        std::mt19937 gen(rd());

        std::uniform_int_distribution<> distr(INT32_MIN, INT32_MAX);

        out << distr(gen) << ' ';
    }

    return 0;
}