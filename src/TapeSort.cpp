#include "TapeSort.hpp"

TapeSort::TapeSort(std::string &input_tape, 
    std::string &output_tape, 
    Config conf, 
    std::shared_ptr<Logger> logger, TapeFactory tape_factory): ram_size(conf.ram_size), 
                                       conf(conf),
                                       logger(logger),
                                       tape_factory(tape_factory)
{
    start_tape = tape_factory(input_tape, true);
    end_tape = tape_factory(output_tape, true);
    temp_files_count = 0;
    logger->write(Event(Stage::sort, "Initial sort"));

    if (!std::filesystem::exists("../tmp/")){
        std::filesystem::create_directories("../tmp/");
    }
}

void TapeSort::split(){
    if (std::filesystem::exists("../tmp/"))
    {
        std::filesystem::remove_all("../tmp/");
    }
    std::filesystem::create_directories("../tmp/");

    size_t capacity = (static_cast<size_t>(ram_size * 0.9) - sizeof(std::vector<int32_t>)) / sizeof(int32_t);

    logger->write(Event(Stage::split, std::format("Calculate capacity: {}", capacity)));

    if (capacity < 1){
        std::cerr << "Error : not enough memory." << std::endl;                                                               
        exit(1);
    }

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

        auto inter_tape = tape_factory(std::format("../tmp/tape_{}.txt", file_num++), true);

        std::for_each(buffer.begin(), buffer.end(), [&](int32_t x){
            inter_tape->write(x);
            inter_tape->move(true);
        });
    }
    
    temp_files_count = file_num;

    logger->write(Event(Stage::split, std::format("Split done. Files created: {}", temp_files_count)));
}

void TapeSort::merge(){
    size_t ones_tape = sizeof(Tape) + sizeof(Node) + sizeof(std::unique_ptr<Tape>);
    size_t capacity = static_cast<size_t>(ram_size * 0.9) / ones_tape;

    logger->write(Event(Stage::merge, std::format("Calculate capacity: {}", capacity)));

    if (capacity < 2) {
        std::cerr << "Error: not enough memory." << std::endl;
        exit(1);
    }

    std::vector<std::unique_ptr<ITape>> tapes;
    tapes.reserve(capacity);
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> minHeap;

    if (capacity >= temp_files_count){
        logger->write(Event(Stage::merge, std::format("Merge from one layer: capacity >= temp_files_count ({} >= {})", capacity, temp_files_count)));
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
                std::unique_ptr<ITape> pt = tape_factory(out_name, true);

                sub_merge(tapes, minHeap, pt, start, range, layers - 1);

                tapes.clear();
                while (!minHeap.empty()) {minHeap.pop();}
                files_in_layers++;
            }
            
            clean_layers(layers - 1, current_temp_files);
            current_temp_files = files_in_layers;
            layers++;
        }

        logger->write(Event(Stage::merge, std::format("Layer: {}, files in layer: {}", layers, current_temp_files)));

        sub_merge(tapes, minHeap, this->end_tape, 0, current_temp_files, layers - 1);
        clean_layers(layers - 1, current_temp_files);

        logger->write(Event(Stage::merge, "Merge done"));
    }
}

void TapeSort::sub_merge(std::vector<std::unique_ptr<ITape>> &tapes, std::priority_queue<Node, std::vector<Node>, std::greater<Node>> &minHeap, std::unique_ptr<ITape>& output_tape, size_t start_range, size_t range, size_t prev_layer)
{
    for (size_t file = 0; file < range; file++)
    {
        std::string inp_name = (prev_layer == 1) ? std::format("../tmp/tape_{}.txt", start_range + file) : std::format("../tmp/layers_{}_{}.txt", prev_layer, start_range + file);
        std::unique_ptr<ITape> t = tape_factory(inp_name, true);
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

    logger->write(Event(Stage::sort, std::format("Clean layer [{}], files[{}]", layer, files_in_layers)));
}


void TapeSort::start(){
    this->split();
    this->merge();
}
