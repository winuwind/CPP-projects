#include "Controller.h"

#include <sstream>

void Controller::InitFunctions() {
    void(Controller::*func)(const std::string &);
    func = & Controller::Exit;
    functions.insert(std::pair(Command::exit, func));
    func = & Controller::Help;
    functions.insert(std::pair(Command::help, func));
    func = & Controller::Draw;
    functions.insert(std::pair(Command::draw, func));
    func = & Controller::Tick;
    functions.insert(std::pair(Command::t, func));
    functions.insert(std::pair(Command::tick, func));
    func = & Controller::Dump;
    functions.insert(std::pair(Command::dump, func));
}

Controller::Controller() :
        model(),
        field(model.GetField())
    {
    out = "out.lif";
    state = State::SimpleMode;
    InitFunctions();
}

Controller::Controller(const std::string &in, std::string & out, unsigned int iterations) :
        model(in),
        out(out),
        field(model.GetField()),
        graphics(model.GetField(), model.GetName())
{
    model.Step(iterations);
    state = State::SimpleMode;
    InitFunctions();
}

void Controller::RunCommand(const std::string &line) {
    std::istringstream in(line);
    std::string command;
    std::string arg;
    in >> command;
    in >> arg;
    Command command_;
    if(command == "exit"){
        command_ = Command::exit;
    }
    else if(command == "tick"){
        command_ = Command::tick;
    }
    else if(command == "t"){
        command_ = Command::t;
    }
    else if(command == "help"){
        command_ = Command::help;
    }
    else if(command == "draw"){
        command_ = Command::draw;
    }
    else if(command == "dump"){
        command_ = Command::dump;
    }
    else{
        Error::Command::print_error_command(command);
        return;
    }
    (this->*functions[command_])(arg);
}

void Controller::Dump(const std::string & filename) {
    model.Save(filename);
}

void Controller::Tick(const std::string & arg) {
    size_t n = 1;
    if(!arg.empty()){
        std::stoull(arg);
    }
    model.Step(n);
}

void Controller::Exit(const std::string & arg) {
    model.Save(out);
    state = State::Completed;
    if(!arg.empty()){
        Error::Command::print_error_argument(arg);
    }
}

void Controller::Draw(const std::string & arg) {
    graphics.Draw(field);
    SDL_ShowWindow(graphics.window);
    SDL_RenderPresent(graphics.renderer);
    state = State::ViewMode;
    if(!arg.empty()){
        Error::Command::print_error_argument(arg);
    }
}

void Controller::Help(const std::string & arg) {
    Error::PrintListOfCommands();
    if(!arg.empty()){
        Error::Command::print_error_argument(arg);
    }
}

void Controller::run() {
    graphics.Draw(field);
    SDL_RenderPresent(graphics.renderer);
    SDL_Event event;
    bool flag_scroll = false;
    while (state != State::Completed) {
        while (state == State::ViewMode) {
            if(flag_scroll){
                model.Step();
                graphics.Draw(field);
                SDL_RenderPresent(graphics.renderer);
            }
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    SDL_HideWindow(graphics.window);
                    state = State::SimpleMode;
                    break;
                }
                if (event.type == SDL_KEYDOWN) {
                    if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
                        SDL_HideWindow(graphics.window);
                        state = State::SimpleMode;
                        break;
                    }
                    else if(event.key.keysym.scancode == SDL_SCANCODE_RIGHT){
                        flag_scroll = true;
                        break;
                    }
                    else if (event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
                        flag_scroll = false;
                        break;
                    }
                    else{
                        model.Step();
                        graphics.Draw(field);
                        SDL_RenderPresent(graphics.renderer);
                    }
                }
                if (event.type == SDL_MOUSEBUTTONDOWN){
                    int x, y;
                    int k = static_cast<int>(graphics.k);
                    SDL_GetMouseState(&x, &y);
                    x /= k; y /= k;
                    Coordinates point(x, y);
                    if(field.Get(point) == Cell::Alive){
                        field.Set(point, Cell::Dead);
                    }
                    else{
                        field.Set(point, Cell::Alive);
                    }
                    graphics.Draw(field);
                    SDL_RenderPresent(graphics.renderer);
                }
            }
        }
        std::string command;
        std::getline(std::cin, command);
        RunCommand(command);
    }
}
