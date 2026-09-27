#pragma once

#include <memory>

#include "Converter.h"

class Creator{
private:
    unsigned start;
    unsigned end;
protected:
    Creator();
public:
    virtual std::unique_ptr<Converter> factoryMethod(unsigned second) = 0;

    virtual ~Creator();

    void setData(unsigned start_, unsigned end_);

    [[nodiscard]] unsigned getStart() const;

    [[nodiscard]] unsigned getEnd() const;
};