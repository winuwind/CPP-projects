#pragma once

#include "Converter.h"

class ConverterMute : public Converter{
public:
    ConverterMute();

    std::vector<short> & Convert(std::vector<short> & data) override;

    ~ConverterMute() override;
};