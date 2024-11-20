#include "Field.h"

Field::Field() :
height(67),
width(67)
{
    Plane = new bool * [height];
    for (int i = 0; i < height; i++) {
        Plane[i] = new bool[width];
        for (int j = 0; j < width; j++) {
            Plane[i][j] = (bool) (((j % 5) + 3) % 2);
        }
    }
}

Field::Field(std::string & filename) :
    height(67),
    width(67)
{
    std::ifstream file;
    file.open(filename.c_str(), std::ios::in);
    std::string line;
    while(std::getline(file, line)){
        if(line.find("#S") == 0){
            std::istringstream is( line );
            std::string help;
            is >> help;
            is >> width;
            is >> height;
            break;
        }
    }
    file.close();
    file.open(filename.c_str(), std::ios::in);
    Plane = new bool * [height];
    for (int i = 0; i < height; i++) {
        Plane[i] = new bool[width];
        for(int j = 0; j < width; j++){
            Plane[i][j] = false;
        }
    }
    while(std::getline(file, line)){
        if(line[0] == '#'){
            continue;
        }
        std::istringstream is( line );
        int x, y;
        is >> x;
        is >> y;
        Plane[y % height][x % width] = true;
    }
    file.close();
}

Field::~Field(){
    Delete();
}

void Field::Delete() {
    if(Field::Plane != nullptr){
        for(int i = 0; i < Field::height; i++){
            if(Field::Plane[i] != nullptr){
                delete[] Field::Plane[i];
                Field::Plane[i] = nullptr;
            }
        }
        delete[] Field::Plane;
        Field::Plane = nullptr;
    }
}

bool Field::Get(Coords point){
    return Field::Plane[point.y][point.x];
}

void Field::Set(Coords point, bool value){
    Plane[point.y][point.x] = value;
}

Coords & Field::GetSize(Coords & size) const {
    size.x = width;
    size.y = height;
    return size;
}