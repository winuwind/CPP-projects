#pragma once

#include "Model.h"
#include "graphics.h"

#undef main

class Controller{
private:
    Field field;
    Rule rules;
    std::string out;
    bool flagWorkGraphics;
    bool flagWorkProgram;
    Graphics graphics;
public:
    Controller();

    Controller(std::string & in, std::string & out, unsigned iterations);

    ~Controller();

    void Save(std::string & filename);

    void GetFilename(std::string & command);

    void Tick(std::string & command);

    void CheckCommand(std::string & command);

    void main();
};














