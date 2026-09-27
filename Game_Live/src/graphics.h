#pragma once

#include "SDL2/SDL.h"
#include "Field.h"

class Graphics{
private:
    void Destroy();
    Coords size;
    std::string name;
public:
    SDL_Renderer* renderer;
    SDL_Window* window;
    unsigned k;

    void Draw(Field & field);

    Graphics();

    explicit Graphics(Field & field, std::string & name);

    ~Graphics();
};

