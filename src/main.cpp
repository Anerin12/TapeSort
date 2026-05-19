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
    generate_input(1000, "../input.txt");
    std::string inp_tape;
    std::string out_tape;

    std::shared_ptr<Logger> logger = nullptr;

    if (argc < 2)
    {
        std::cerr << "Path for input/output not found.\n";
        exit(1);
    }
    else
    {
        if (checkInput(argv[1])){
            inp_tape = argv[1];
        }
        else {
            exit(1);
        }

        if (argc > 2){
            if (checkInput(argv[2])){}
            out_tape = argv[2];
        }
        else out_tape = "../output.txt";
    }

    logger = std::make_shared<Logger>("../logs/log.txt");

    Config conf = parseConfig("../config/config.txt");

    TapeFactory tape_factory = [conf, logger](const std::string &name,  bool logging) -> std::unique_ptr<ITape>
    {
        if (logging){
            return std::make_unique<Tape>(name, conf, logger);
        }
        return std::make_unique<Tape>(name, conf, nullptr);
    };

    TapeSort sort = TapeSort(inp_tape, out_tape, conf, logger, tape_factory);

    sort.start();

    return 0;
}