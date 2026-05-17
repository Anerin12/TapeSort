#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

struct Config
{
    size_t rw_delay = 0;
    size_t rewind_delay = 0;
    size_t move_delay = 0;
    size_t ram_size = 0;
};

Config parseConfig(const std::string &filename);