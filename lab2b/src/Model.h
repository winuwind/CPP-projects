#pragma once

#include <vector>
#include "Field.h"
#include "Errors.h"

class Rule{
public:
    std::vector<bool> forLive;
    std::vector<bool> forBirth;
    std::string Name;
    Coords size;
    void Init();
    Rule();

    explicit Rule(std::string & filename);

    void Step(Field & field);

    void Step(Field & field, unsigned n);
};

