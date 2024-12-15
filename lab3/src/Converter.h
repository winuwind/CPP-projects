#pragma once

#include <vector>

class Converter{
public:
    virtual std::vector<short> & Convert(std::vector<short> & data) = 0;

    virtual ~Converter() = default;
};