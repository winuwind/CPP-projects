#pragma once

struct Coords{
    unsigned x, y;

    Coords() = default;

    Coords(unsigned x_, unsigned y_) :
        x(x_),
        y(y_){}

    Coords& operator=(const Coords& other) = default;
};


