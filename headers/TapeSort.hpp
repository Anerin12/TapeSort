#pragma once
#include <vector>
#include <algorithm>
#include <functional>
#include <format>
#include <queue>
#include <filesystem>
#include <iostream>
#include "Tape.hpp"

using TapeFactory = std::function<std::unique_ptr<ITape>(const std::string &name, bool logging)>;

struct Node
{
    int32_t value;
    size_t tape_idx;

    bool operator>(const Node &other) const
    {
        if (value == other.value)
        {
            return tape_idx > other.tape_idx; 
        }

        return value > other.value;
    }
};

class TapeSort
{
    private:
        size_t ram_size;
        size_t temp_files_count;
        Config conf;
        TapeFactory tape_factory;
        std::shared_ptr<Logger> logger;
        std::unique_ptr<ITape> start_tape;
        std::unique_ptr<ITape> end_tape;

        void split();
        void merge();
        void sub_merge(std::vector<std::unique_ptr<ITape>> &tapes, std::priority_queue<Node, std::vector<Node>, std::greater<Node>> &minHeap, std::unique_ptr<ITape> &output_tape, size_t start_range, size_t range, size_t prev_layer);
        void clean_layers(size_t layer, size_t files_in_layers);

    public:
        TapeSort(std::string &input_tape, std::string &output_tape, Config conf, std::shared_ptr<Logger> logger, TapeFactory tape_factory);
        void start();
};

