#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

struct Config
{
    int rw_delay = 0;
    int rewind_delay = 0;
    int move_delay = 0;
    size_t ram_size = 0;
};

Config parseConfig(const std::string &filename);