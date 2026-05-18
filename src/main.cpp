#include "TapeSort.hpp"
#include "generator.hpp"

bool checkInput(std::string file_name){
    if (!std::filesystem::exists(file_name)){
        std::cout << std::format("Tape ({}) damaged or non-existent. Try again.", file_name) << std::endl;
        return false;
    }

    return true;
}

int main(int argc, char *argv[])
{
    std::shared_ptr<Logger> logger = nullptr;

    if (argc < 2)
    {
        std::cout << "Path for logging not found. App continues without logging.\n";
    }
    else
    {
        logger = std::make_shared<Logger>(argv[1]);
    }

    std::string inp_tape;
    std::string out_tape;

    std::cout << "Welcome to TapeSort!\nYou can use a ready-made feed or generate one here:\n1. Import tape\n2. Generate tape" << std::endl;
    int opt;
    std::cin >> opt;

    switch (opt)
    {
    case 1:
        std::cout << "Input path for start tape: ";
        std::cin >> inp_tape;

        while (!checkInput(inp_tape))
        {
            std::cout << "Input path for start tape: ";
            std::cin >> inp_tape;
        }
        break;

    case 2:
        std::cout << "Input path for start tape: ";
        std::cin >> inp_tape;
        std::cout << "Input count munbers in tape: ";
        size_t size;
        std::cin >> size;
        generate_input(size, inp_tape);
        break;

    default:
        break;
    }

    std::cout << "Input path for result tape: ";
    std::cin >> out_tape;

    Config conf = parseConfig("../config/config.txt");
    TapeSort sort = TapeSort(inp_tape, out_tape, conf, logger);

    sort.start();

    return 0;
}