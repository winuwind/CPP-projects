#pragma once

#include <bitset>

#include "Field.h"
#include "Errors.h"

class Model{
private:
    void Init();
    std::bitset<9> forLive;
    std::bitset<9> forBirth;
    Size size;
    std::string Name;
    Field field;
public:

    Model();

    explicit Model(const std::string & filename);

    void Step();

    void Step(unsigned n);

    void Save(const std::string & filename);

    std::string & GetName();

    Field & GetField();
};

