#pragma once

#include <fstream>
#include <sstream>
#include "Coordinates.h"

class Field{
private:
    unsigned height;
    unsigned width;
    bool** Plane;
    void Delete();
public:
    Field();

    explicit Field(std::string & file);

    bool Get(Coords point);

    void Set(Coords point, bool value);

    [[nodiscard]] Coords & GetSize(Coords & size) const;

    ~Field();
};






