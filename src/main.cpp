#include "TapeSort.hpp"

// Конвертируем числа через пробел из входного файла в формат со строгим колличеством байт (12)
// для того что бы эмулировать движение по ленте.
void convertToTapeFormat(const std::string &in_path, const std::string &out_path)
{
    std::ifstream in(in_path);
    std::ofstream out(out_path);
    int32_t val;
    while (in >> val)
    {
        out << std::setw(11) << std::setfill(' ') << val << " ";
    }
}

int main() {
    
    std::string inp_tape;
    std::string out_tape; 

    std::cout << "Input path for start tape: ";
    std::cin >> inp_tape;
    std::cout << "Input path for result tape: ";
    std::cin >> out_tape;

    Config conf = parseConfig("../config/config.txt");

    TapeSort sort = TapeSort(inp_tape, out_tape, conf);

    sort.start();

    return 0;
}