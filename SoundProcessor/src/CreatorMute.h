#pragma once

#include "Creator.h"
#include "ConverterMute.h"

class CreatorMute : public Creator{
public:
    explicit CreatorMute(std::string & line);

    std::unique_ptr<Converter> factoryMethod(unsigned second) override;

    ~CreatorMute() override;
};