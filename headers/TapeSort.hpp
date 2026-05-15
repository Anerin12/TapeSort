#include <vector>
#include <algorithm>
#include <format>
#include <queue>
#include <filesystem>
#include <iostream>
#include "Tape.hpp"

struct Node
{
    int32_t value;
    size_t tape_idx;

    bool operator>(const Node &other) const
    {
        return value > other.value;
    }
};

class TapeSort
{
    private:
        size_t ram_size;
        size_t temp_files_count;
        Config conf;
        std::unique_ptr<Tape> start_tape;
        std::unique_ptr<Tape> end_tape;

        void split();
        void merge();
        void sub_merge(std::vector<std::unique_ptr<Tape>> &tapes, std::priority_queue<Node, std::vector<Node>, std::greater<Node>> &minHeap, std::unique_ptr<Tape> &output_tape, size_t start_range, size_t range, size_t prev_layer);
        void clean_layers(size_t layer, size_t files_in_layers);

    public:
        TapeSort(std::string &input_tape, std::string &output_tape, Config conf);
        void start();
};

