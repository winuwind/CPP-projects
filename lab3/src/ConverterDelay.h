#pragma once

#include "Converter.h"

class ConverterDelay : public Converter{
private:
    std::vector<short> dataPrev;
    double dryLevel;
    double wetLevel;
    double feedback;
public:
    ConverterDelay(std::vector<short> & dataPrev_, double dryLevel_, double wetLevel_, double feedback_);

    std::vector<short> & Convert(std::vector<short> & data) override;

    ~ConverterDelay() override;
};