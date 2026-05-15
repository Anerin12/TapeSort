#include "TapeSort.hpp"

TapeSort::TapeSort(std::string &input_tape, std::string &output_tape, Config conf): ram_size(conf.ram_size), conf(conf){
    start_tape = std::make_unique<Tape>(input_tape, conf);
    end_tape = std::make_unique<Tape>(output_tape, conf);
    temp_files_count = 0;
}

void TapeSort::split(){
    size_t capacity = (static_cast<size_t>(ram_size * 0.9) - sizeof(std::vector<int32_t>)) / sizeof(int32_t);
    std::cout << "Capacit in split: " << capacity << std::endl;
    std::vector<int32_t> buffer;
    buffer.reserve(capacity);

    size_t file_num = 0;
    bool end_of_tape = false;
    
    while (!end_of_tape) {
        buffer.clear();

        for (size_t num = 0; num < capacity; num++)
        {
            auto val = this->start_tape->read();

            if (!val){
                end_of_tape = true;
                break;
            }

            buffer.push_back(*val);
            this->start_tape->move(true);
        }

        if (buffer.empty()) break;

        std::sort(buffer.begin(), buffer.end());

        Tape inter_tape(std::format("../tmp/tape_{}.txt", file_num++), conf);

        std::for_each(buffer.begin(), buffer.end(), [&](int32_t x){
            inter_tape.write(x);
            inter_tape.move(true);
        });
    }
    
    temp_files_count = file_num;
}

void TapeSort::merge(){
    size_t ones_tape = sizeof(Tape) + sizeof(Node) + sizeof(std::unique_ptr<Tape>);
    std::cout << "Размер одного объекта Tape в RAM: " << sizeof(Tape) << " байт" << std::endl;
    size_t capacity = static_cast<size_t>(ram_size * 0.9) / ones_tape;
    std::cout << "Capacity: " << capacity << std::endl;
    std::vector<std::unique_ptr<Tape>> tapes;
    tapes.reserve(capacity);
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> minHeap;

    if (capacity >= temp_files_count){
        sub_merge(tapes, minHeap, this->end_tape, 0, this->temp_files_count, 1);
    }
    else {
        // Логика для мержа с промежуточными лентами (если 1 элемент каждой линии не помещается в оперативку)
        size_t current_temp_files = temp_files_count;
        size_t layers = 2;

        while (current_temp_files > capacity){
            size_t files_in_layers = 0;

            for (size_t start = 0; start < current_temp_files; start += capacity){
                size_t range = std::min(capacity, current_temp_files - start);

                std::string out_name = std::format("../tmp/layers_{}_{}.txt", layers, files_in_layers);
                std::unique_ptr<Tape> pt = std::make_unique<Tape>(out_name, conf);

                sub_merge(tapes, minHeap, pt, start, range, layers - 1);

                tapes.clear();
                while (!minHeap.empty()) {minHeap.pop();}
                files_in_layers++;
            }
            
            clean_layers(layers - 1, current_temp_files);
            current_temp_files = files_in_layers;
            layers++;
        }

        sub_merge(tapes, minHeap, this->end_tape, 0, current_temp_files, layers - 1);
        clean_layers(layers - 1, current_temp_files);
    }
}

void TapeSort::sub_merge(std::vector<std::unique_ptr<Tape>> &tapes, std::priority_queue<Node, std::vector<Node>, std::greater<Node>> &minHeap, std::unique_ptr<Tape>& output_tape, size_t start_range, size_t range, size_t prev_layer)
{
    for (size_t file = 0; file < range; file++)
    {
        std::string inp_name = (prev_layer == 1) ? std::format("../tmp/tape_{}.txt", start_range + file) : std::format("../tmp/layers_{}_{}.txt", prev_layer, start_range + file);
        std::unique_ptr<Tape> t = std::make_unique<Tape>(inp_name, conf);
        auto val = t->read();
        if (val){
            minHeap.push({*val, file});
            tapes.push_back(std::move(t));
        }
    }

    while (!minHeap.empty())
    {
        Node min_node = minHeap.top();
        minHeap.pop();
        output_tape->write(min_node.value);
        output_tape->move(1);

        auto &t = tapes[min_node.tape_idx];
        t->move(1);
        auto val = t->read();
        if (val)
        {
            Node node = {val.value(), min_node.tape_idx};
            minHeap.push(node);
        }
    }
}

void TapeSort::clean_layers(size_t layer, size_t files_in_layers){
    for (size_t num = 0; num < files_in_layers; num++){
        std::filesystem::path file_path = (layer == 1) ? std::format("../tmp/tape_{}.txt", num) : std::format("../tmp/layers_{}_{}.txt", layer, num);

        try {
            std::filesystem::remove(file_path);
        }
        catch (const std::filesystem::filesystem_error &err){
            std::cerr << "Error deleted files: " << err.what() << std::endl;
        }
    }
}


void TapeSort::start(){
    this->split();
    this->merge();
}
