#pragma once

#include <vector>
#include <string>

#include "Coordinates.h"

enum class Cell{
    Dead,
    Alive
};

struct Size{
    unsigned width;
    unsigned height;
};

class Field{
private:
    Size size;
    std::vector<Cell> plane;

    void Init();
public:
    Field();

    explicit Field(Size size_);

    Cell Get(Coordinates point);

    void Set(Coordinates point, Cell state);

    [[nodiscard]] Size & GetSize();
};
