#pragma once

#include "Converter.h"

class ConverterMix : public Converter{
private:
    std::vector<short> dataAdd;
public:
    explicit ConverterMix(std::vector<short> & dataAdd_);

    std::vector<short> & Convert(std::vector<short> & data) override;

    ~ConverterMix() override;
};