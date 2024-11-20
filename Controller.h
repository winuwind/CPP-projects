#pragma once

#include "Configuration.h"

class Controller{
private:
    WAV & wav;
    std::vector<std::string> arr_in;
    std::string filepath_out;
    Configuration config;
public:
    Controller() = delete;

    explicit Controller(std::string & filepath_out, std::string & filepath_config, std::vector<std::string> & arr_in, WAV & wav);

    void main();
};