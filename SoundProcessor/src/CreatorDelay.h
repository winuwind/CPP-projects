#pragma once

#include "Creator.h"
#include "ConverterDelay.h"

class CreatorDelay : public Creator{
private:
    std::string filepath;
    double dryLevel;
    double wetLevel;
    double feedback;
public:
    CreatorDelay(std::vector<std::string> & arrIn,std::string & line);

    std::unique_ptr<Converter> factoryMethod(unsigned second) override;

    ~CreatorDelay() override;
};