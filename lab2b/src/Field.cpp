#include "Field.h"

void Field::Init(){
    plane = std::vector<Cell>(size.width * size.height);
    for(unsigned i = 0; i < size.width * size.height; i++){
        plane[i] = Cell::Dead;
    }
}

Field::Field():
        size(67, 67)
{
    Init();
}

Field::Field(Size size_):
        size(size_)
{
    Init();
}

Cell Field::Get(Coordinates point){
    return Field::plane[(point.y % size.height) * size.width + (point.x % size.width)];
}

void Field::Set(Coordinates point, Cell state){
    plane[(point.y % size.height) * size.width + (point.x % size.width)] = state;
}

Size & Field::GetSize() {
    Size & size_ref(size);
    return size_ref;
}
