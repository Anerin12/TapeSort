#include <catch2/catch_test_macros.hpp>
#include <random>
#define private public
#include "generator.hpp"

inline auto logger_mock = std::make_shared<Logger>("test_log.txt");


bool checkSort(std::string filename){

    std::ifstream input(filename);
    if (!input.is_open())
        return true;

    int32_t prev;
    if (!(input >> prev))
    {
        return true; 
    }

    int32_t val;
    while (input >> val)
    {
        if (prev > val)
        {
            return false;
        }
        prev = val;
    }

    return true;
}

int countFiles(std::string nameDir){
    std::filesystem::path dirPath = nameDir; 
    size_t fileCount = 0;

    if (std::filesystem::exists(dirPath) && std::filesystem::is_directory(dirPath))
    {
        for (const auto &entry : std::filesystem::directory_iterator(dirPath))
        {
            if (std::filesystem::is_regular_file(entry.status()))
            {
                fileCount++;
            }
        }
        return fileCount ;
    }
    else
    {
        std::cerr << "Путь не существует или это не директория." << std::endl;
    }
    return 0;
}

void deleteArtifact(){
    std::filesystem::remove("./input.txt");
    std::filesystem::remove("./output.txt");
}

// Метод split()
TEST_CASE("Correct split input file", "[split]"){
    generate_input(100, "./input.txt");
    std::string inp = "./input.txt";
    std::string out = "./output.txt";
    Config conf = {0, 0, 0, 72};

    TapeFactory mock_factory = [conf](const std::string &name, bool logging)
    {
        return std::make_unique<Tape>(name, conf);
    };

    TapeSort sort(inp, out, conf, logger_mock, mock_factory); // При 72 байтах памяти, 100 чисел должно разделиться ровно на 10 отсортированных лент

    SECTION("Correct split input tape"){
        sort.split();

        REQUIRE(checkSort("../tmp/tape_0.txt"));
        REQUIRE(countFiles("../tmp/") == 10);

        sort.clean_layers(1, 10);
        deleteArtifact();
    }
}

// Метод merge()
TEST_CASE("Correct work merge loop (capacity > ram_size)", "[merge]"){
    generate_input(100, "./input.txt");
    std::string inp = "./input.txt";
    std::string out = "./output.txt";
    Config conf = {0, 0, 0, 2048};

    TapeFactory mock_factory = [conf](const std::string &name, bool logging)
    {
        return std::make_unique<Tape>(name, conf);
    };

    TapeSort sort(inp, out, conf, logger_mock, mock_factory);
    sort.split();

    REQUIRE(sort.temp_files_count == 1);

    sort.merge();

    REQUIRE(checkSort(out));
    deleteArtifact();
}

TEST_CASE("Correct work merge loop (capacity < ram_size)", "[merge]")
{
    generate_input(10000, "./input.txt");
    std::string inp = "./input.txt";
    std::string out = "./output.txt";
    Config conf = {0, 0, 0, 2048};

    TapeFactory mock_factory = [conf](const std::string &name, bool logging)
    {
        return std::make_unique<Tape>(name, conf);
    };

    TapeSort sort(inp, out, conf, logger_mock, mock_factory);
    sort.split();

    sort.merge();

    REQUIRE(checkSort(out));

    REQUIRE(countFiles("../tmp/") == 0);
    deleteArtifact();
}

#undef private