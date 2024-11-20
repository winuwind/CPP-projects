#include "Model.h"

Rule::Rule() :
    size(67, 67)
{
    Init();
}

Rule::Rule(std::string & filename):
        size(67, 67)
{
    Init();
    std::string line;
    std::ifstream file;
    file.open(filename.c_str(), std::ios::in);
    std::getline(file, line);
    if(line != "#Life 1.06"){
        Error::File::error_format(filename);
        file.close();
        return;
    }
    else {
        while (std::getline(file, line)) {
            if (line.find("#N ") == 0) {
                Name.clear();
                for (size_t i = 3; i < line.length(); i++) {
                    Name.push_back(line[i]);
                }
            }
            if (line.find("#R B") == 0) {
                size_t start = 4;
                for (size_t i = start; i < line.length(); i++) {
                    if (isdigit(line[i])) {
                        forBirth[static_cast<unsigned>(line[i] - '0')] = true;
                    } else if (i + 1 < line.length() && line[i] == '/' && line[i + 1] == 'S') {
                        start = i + 2;
                        break;
                    } else {
                        Error::File::error_format(filename);
                        file.close();
                        return;
                    }
                }
                for (size_t i = start; i < line.length(); i++) {
                    if (isdigit(line[i])) {
                        forLive[static_cast<unsigned>(line[i] - '0')] = true;
                    } else {
                        Error::File::error_format(filename);
                        file.close();
                        return;
                    }
                }
            }
            if(line.find("#S") == 0){
                std::istringstream is( line );
                std::string help;
                is >> help;
                is >> size.x;
                is >> size.y;
            }
        }
    }
    file.close();
}

void Rule::Init() {
    forBirth.resize(9);
    forLive.resize(9);
    for(int i = 0; i < 9; i++){
        forLive[i] = false;
        forBirth[i] = false;
    }
    Name = "ExampleLive";
}

void Rule::Step(Field & field) {
    std::vector<Coords> arr_coords;
    unsigned height_ = size.y, width_ = size.x;
    for(int y = 0; y < height_; y++){
        for(int x = 0; x < width_; x++){
            Coords point(x, y);
            Coords point1((x + width_ - 1) % width_, (y + height_ - 1) % height_);
            Coords point2((x) % width_, (y + height_ - 1) % height_);
            Coords point3((x + 1) % width_, (y + height_ - 1) % height_);
            Coords point4((x + width_ - 1) % width_, (y) % height_);
            Coords point5((x + 1) % width_, (y) % height_);
            Coords point6((x + width_ - 1) % width_, (y + 1) % height_);
            Coords point7((x) % width_, (y + 1) % height_);
            Coords point8((x + 1) % width_, (y + 1) % height_);
            int count = static_cast<int>(field.Get(point1));
            count += static_cast<int>(field.Get(point2));
            count += static_cast<int>(field.Get(point3));
            count += static_cast<int>(field.Get(point4));
            count += static_cast<int>(field.Get(point5));
            count += static_cast<int>(field.Get(point6));
            count += static_cast<int>(field.Get(point7));
            count += static_cast<int>(field.Get(point8));
            if(forLive[count] && field.Get(point) || forBirth[count] && !field.Get(point)){
                Coords cell(x, y);
                arr_coords.push_back(cell);
            }
        }
    }
    for(int y = 0; y < height_; y++){
        for(int x = 0; x < width_; x++){
            Coords point(x, y);
            field.Set(point, false);
        }
    }
    for(auto arr_coord : arr_coords){
        field.Set(arr_coord, true);
    }
}

void Rule::Step(Field & field, unsigned n){
    while(n--) {
        Step(field);
    }
}