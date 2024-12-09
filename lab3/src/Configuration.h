#pragma once

#include "Converters.h"
#include <vector>

class Configuration{
private:
    std::vector<std::unique_ptr<Creator>> creators;
public:
    Configuration();

    explicit Configuration(std::string & filepath, std::vector<std::string> & arr_in, WAV & wav);

    static void PrintSyntax();

    std::vector<std::unique_ptr<Creator>> & GetTransformations();
};
