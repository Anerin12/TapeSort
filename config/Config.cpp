#include "Config.hpp"

Config parseConfig(const std::string &filename)
{
    Config config;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open())
    {
        std::cerr << "Не удалось открыть файл конфигурации!" << std::endl;
        return config;
    }

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        size_t delimiterPos = line.find('=');
        if (delimiterPos != std::string::npos)
        {
            std::string key = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 1);

            if (key == "rw_delay")
                config.rw_delay = std::stoi(value);
            else if (key == "rewind_delay")
                config.rewind_delay = std::stoi(value);
            else if (key == "move_delay")
                config.move_delay = std::stoi(value);
            else if (key == "ram_size")
                config.ram_size = std::stoull(value);
        }
    }

    return config;
}