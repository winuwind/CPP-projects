#pragma once

#include "Converters.h"
#include <sstream>

class Configuration{
public:
    std::vector<Creator*> creators;

    Configuration();

    explicit Configuration(std::string & filepath, std::vector<std::string> & arr_in, WAV & wav);

    static void PrintSyntax();
};