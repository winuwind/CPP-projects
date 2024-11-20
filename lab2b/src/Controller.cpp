#include "Controller.h"

Controller::Controller() {
    out = "out.lif";
    flagWorkProgram = true;
    flagWorkGraphics = false;
}

Controller::Controller(std::string &in, std::string &out, unsigned int iterations) :
        rules(in),
        field(in),
        graphics(field, rules.Name),
        out(out),
        flagWorkGraphics(false),
        flagWorkProgram(true)
{
    rules.Step(field, iterations);
}

Controller::~Controller() = default;

void Controller::Save(std::string &filename) {
    std::ofstream file(filename, std::ios::out);
    file.write("#Life 1.06\n", 11);
    file.write("#N ", 3);
    file.write(rules.Name.c_str(), static_cast<int>(rules.Name.length()));
    file.write("\n", 1);
    file.write("#S ", 3);
    file.write(std::to_string(rules.size.x).c_str(), std::to_string(rules.size.x).length());
    file.write(" ", 1);
    file.write(std::to_string(rules.size.y).c_str(), std::to_string(rules.size.y).length());
    file.write("\n", 1);
    file.write("#R B", 4);
    for(int i = 0; i <= 9; i++){
        if(rules.forBirth[i]){
            file.write(std::to_string(i).c_str(), 1);
        }
    }
    file.write("/S", 2);
    for(int i = 0; i <= 9; i++){
        if(rules.forLive[i]){
            file.write(std::to_string(i).c_str(), 1);
        }
    }
    file.write("\n", 1);
    Coords size(0, 0);
    size = field.GetSize(size);
    for(int y = 0; y < size.y; y++){
        for(int x = 0; x < size.x; x++){
            Coords point(x, y);
            if(field.Get(point)){
                std::string line(std::to_string(x));
                line.append(" ");
                line.append(std::to_string(y));
                line.append("\n");
                file.write(line.c_str(), (int) line.length());
            }
        }
    }
    file.close();
}

void Controller::GetFilename(std::string &command) {
    std::string filename;
    for(size_t i = 5; i < command.size(); i++){
        filename.push_back(command[i]);
    }
    Save(filename);
}

void Controller::Tick(std::string &command) {
    std::string n_string;
    size_t delta = 2;
    int n;
    if(command.find("tick") == 0){
        delta = 5;
    }
    if(command == "tick" || command == "t"){
        n = 1;
    }
    else {
        for (size_t i = delta; i < command.size(); i++) {
            if (!isdigit(command[i])) {
                Error::Command::print_error(command);
                break;
            }
            n_string.push_back(command[i]);
        }
        n = std::stoi(n_string);
    }
    rules.Step(field, n);
}

void Controller::CheckCommand(std::string &command) {
    if(command.find("dump") == 0){
        GetFilename(command);
    }
    else if(command.find('t') == 0){
        Tick(command);
    }
    else if(command == "exit"){
        Save(out);
        flagWorkProgram = false;
    }
    else if(command == "help"){
        Error::PrintListOfCommands();
    }
    else if(command == "draw"){
        graphics.Draw(field);
        SDL_ShowWindow(graphics.window);
        flagWorkGraphics = true;
        SDL_RenderPresent(graphics.renderer);
    }
    else{
        Error::Command::print_error(command);
    }
}

void Controller::main() {
    graphics.Draw(field);
    SDL_RenderPresent(graphics.renderer);
    SDL_Event event;
    bool flag_scroll = false;
    while (flagWorkProgram) {
        while (flagWorkGraphics) {
            if(flag_scroll){
                rules.Step(field);
                graphics.Draw(field);
                SDL_RenderPresent(graphics.renderer);
            }
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    SDL_HideWindow(graphics.window);
                    flagWorkGraphics = false;
                    break;
                }
                if (event.type == SDL_KEYDOWN) {
                    if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
                        SDL_HideWindow(graphics.window);
                        flagWorkGraphics = false;
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
                        rules.Step(field);
                        graphics.Draw(field);
                        SDL_RenderPresent(graphics.renderer);
                    }
                }
                if (event.type == SDL_MOUSEBUTTONDOWN){
                    int x, y;
                    int k = static_cast<int>(graphics.k);
                    SDL_GetMouseState(&x, &y);
                    x /= k; y /= k;
                    Coords point(x, y);
                    field.Set(point, !field.Get(point));
                    graphics.Draw(field);
                    SDL_RenderPresent(graphics.renderer);
                }
            }
        }
        std::string command;
        std::getline(std::cin, command);
        CheckCommand(command);
    }
}