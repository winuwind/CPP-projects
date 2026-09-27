#pragma once

#include "Creator.h"
#include "ConverterMix.h"

class CreatorMix : public Creator{
private:
    std::string filepathInAdd;
public:
    CreatorMix(std::vector<std::string> & arrIn, std::string & line);

    std::unique_ptr<Converter> factoryMethod(unsigned second) override;

    ~CreatorMix() override;
};