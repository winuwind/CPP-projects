#include "graphics.h"

void Graphics::Draw(Field &field) {
    Uint8 red = 0, green = 0, blue = 0;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    for(int y = 0; y < size.y; y++){
        for(int x = 0; x < size.x; x++){
            Coords point(x, y);
            if(field.Get(point)){
                red = 0;
                green = 255;
                blue = 0;
            }
            else{
                red = 255;
                green = 255;
                blue = 255;
            }
            SDL_SetRenderDrawColor(renderer, red, green, blue, SDL_ALPHA_OPAQUE);
            SDL_Rect rect = {static_cast<int>(x * k), static_cast<int>(y * k), static_cast<int>(k - 1), static_cast<int>(k - 1)};
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}
Graphics::Graphics() :
    size(67, 67),
    name("ExampleGame")
    {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(name.c_str(),
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 67 * 10 - 1, 67 * 10 - 1, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_HideWindow(window);
    k = 10;
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, SDL_ALPHA_OPAQUE);
    for(int x = 10; x < 67; x += 10){
        SDL_RenderDrawLine(renderer, static_cast<int>(x * k + k - 1), 0, static_cast<int>(x * k + k - 1), static_cast<int>(67 * k - 1));
    }
    for(int y = 10; y < 67; y += 10){
        SDL_RenderDrawLine(renderer, 0, static_cast<int>(y * k + k - 1), static_cast<int>(67 * k - 1), static_cast<int>(y * k + k - 1));
    }
}

Graphics::Graphics(Field &field, std::string & name):
    size(0, 0)
{
    size = field.GetSize(size);
    if(!size.x || !size.y){
        k = 0;
        window = nullptr;
        renderer = nullptr;
        return;
    }
    k = 670 / size.y;
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(name.c_str(),
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              static_cast<int>(size.x * k - 1), static_cast<int>(size.y * k - 1), 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_HideWindow(window);
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, SDL_ALPHA_OPAQUE);
    for(int x = 10; x < size.x; x += 10){
        SDL_RenderDrawLine(renderer, static_cast<int>(x * k + k - 1), 0, static_cast<int>(x * k + k - 1), static_cast<int>(size.y * k - 1));
    }
    for(int y = 10; y < size.y; y += 10){
        SDL_RenderDrawLine(renderer, 0, static_cast<int>(y * k + k - 1), static_cast<int>(size.x * k - 1), static_cast<int>(y * k + k - 1));
    }
    this->name = name;
}

//Graphics & Graphics::operator=(const Graphics & other){
//    if(this == &other){
//        return *this;
//    }
//    Destroy();
//    k = other.k;
//    name = other.name;
//    size = other.size;
//    SDL_Init(SDL_INIT_VIDEO);
//    window = SDL_CreateWindow(other.name.c_str(),
//                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
//                              static_cast<int>(size.x * k - 1), static_cast<int>(size.y * k - 1), 0);
//    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
//    SDL_RenderClear(renderer);
//    SDL_HideWindow(window);
//    SDL_SetRenderDrawColor(renderer, 255, 0, 255, SDL_ALPHA_OPAQUE);
//    for(int x = 6; x < size.x; x += 10){
//        SDL_RenderDrawLine(renderer, static_cast<int>(x * k + k - 1), 0, static_cast<int>(x * k + k - 1), static_cast<int>(size.y * k - 1));
//    }
//    for(int y = 6; y < size.y; y += 10){
//        SDL_RenderDrawLine(renderer, 0, static_cast<int>(y * k + k - 1), static_cast<int>(size.x * k - 1), static_cast<int>(y * k + k - 1));
//    }
//    return *this;
//}

void Graphics::Destroy() {
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    SDL_DestroyWindow(window);
    window = nullptr;
    SDL_Quit();
}

Graphics::~Graphics(){
    Destroy();
}