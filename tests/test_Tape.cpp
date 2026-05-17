#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include "Tape.hpp"

// Метод read()/write()
TEST_CASE("write and read some number", "[r/w]") {
    std::string file = "./test_tape.txt";
    std::filesystem::remove(file);

    {
        Tape example("./test_tape.txt", {0, 0, 0, 0});

        SECTION("Simple number (not critical, positive)"){
            example.write(15);
            int32_t res = *example.read();
            REQUIRE(res == 15);
        }

        SECTION("Simple number (not critical, negative)")
        {
            example.write(-15);
            int32_t res = *example.read();
            REQUIRE(res == -15);
        }

        SECTION("Critical number (positive)")
        {
            example.write(INT32_MAX);
            int32_t res = *example.read();
            REQUIRE(res == INT32_MAX);
        }

        SECTION("Critical number (negative)")
        {
            example.write(INT32_MIN);
            int32_t res = *example.read();
            REQUIRE(res == INT32_MIN);
        }

        SECTION("Read EOT (end of type)"){
            REQUIRE(example.read() == std::nullopt);
        }
    }

    std::filesystem::remove(file);
}

// Метод rewind()

TEST_CASE("Correct rewind to start", "[rewind]"){
    std::string file = "./test_tape.txt";
    std::filesystem::remove(file);

    {
        Tape example = Tape("./test_tape.txt", {0, 0, 0, 0});

        SECTION("Write and move to 3 numbers and do rewind"){
            for (int i = 0; i < 3; i++){
                example.write(i);
                example.move(true);
            }
            example.rewind();
            REQUIRE(*example.read() == 0);
        }
    }

    std::filesystem::remove(file);
}


// Метод move()

TEST_CASE("Correct move from tape", "[move]"){
    std::string file = "./test_tape.txt";
    std::filesystem::remove(file);

    {
        Tape example = Tape("./test_tape.txt", {0, 0, 0, 0});

        SECTION("Check move forward"){
            for (int i = 0; i < 5; i++){
                example.write(i);
                example.move(true);
            }
            example.rewind();

            for (int i = 0; i < 5; i++){
                REQUIRE(*example.read() == i);
                example.move(true);
            }
        }

        SECTION("Check move back"){
            for (int i = 0; i < 5; i++){
                example.write(i);
                example.move(true);
            }
            for (int i = 4; i >= 0; i--){
                example.move(false);
                REQUIRE(*example.read() == i);
            }

            REQUIRE(example.move(false) == false);
        }
    }

    std::filesystem::remove(file);
}