#pragma once

#include "Model.h"
#include "graphics.h"

#include <map>

enum class State{
    Completed,
    ViewMode,
    SimpleMode
};

enum class Command{
    dump,
    tick,
    t,
    help,
    exit,
    draw
};

class Controller{
private:
    Model model;
    std::string out;
    State state;
    Graphics graphics;
    Field & field;
    std::map<Command, void(Controller::*)(const std::string &)> functions;
    friend Model;

    void InitFunctions();
public:
    Controller();

    Controller(const std::string & in, std::string & out, unsigned iterations);

    void Dump(const std::string & arg);

    void Tick(const std::string & arg);

    void Help(const std::string & arg);

    void Draw(const std::string & arg);

    void Exit(const std::string & arg);

    void RunCommand(const std::string & command);

    void run();
};
