#include "Model.h"

#include <fstream>
#include <sstream>

Model::Model() :
        size(67, 67)
{
    Init();
    field = Field(size);
}

Model::Model(const std::string & filename):
        size(67, 67)
{
    Init();
    field = Field(size);
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
                Name = line.substr(3, line.length() - 3);
            }
            else if (line.find("#R B") == 0) {
                size_t start = 4;
                for (size_t i = start; i < line.length(); i++) {
                    if (isdigit(line[i])) {
                        forBirth.set(static_cast<unsigned>(line[i] - '0'));
                    }
                    else if (i + 1 < line.length() && line[i] == '/' && line[i + 1] == 'S') {
                        start = i + 2;
                        break;
                    }
                    else {
                        Error::File::error_format(filename);
                        file.close();
                        return;
                    }
                }
                for (size_t i = start; i < line.length(); i++) {
                    if (isdigit(line[i])) {
                        forLive.set(static_cast<unsigned>(line[i] - '0'));
                    }
                    else {
                        Error::File::error_format(filename);
                        file.close();
                        return;
                    }
                }
            }
            else if(line.find("#S") == 0){
                std::istringstream is( line );
                std::string help;
                is >> help;
                is >> size.width;
                is >> size.height;
                field = Field(size);
            }
            else if(line[0] == '#'){
                Error::File::error_data();
            }
            else{
                std::istringstream is( line );
                int x;
                int y;
                is >> x;
                is >> y;
                Coordinates point(x, y);
                field.Set(point, Cell::Alive);
            }
        }
    }
    file.close();
}

void Model::Init() {
    for(int i = 0; i < 9; i++){
        forLive.reset(i);
        forBirth.reset(i);
    }
    Name = "ExampleLive";
    size.height = 67;
    size.width = 67;
}

void Model::Step() {
    std::vector<Coordinates> arr_Coordinates;
    unsigned height_ = size.height, width_ = size.width;
    Field field_new(size);
    for(int y = 0; y < height_; y++){
        for(int x = 0; x < width_; x++){
            Coordinates point(x, y);
            Coordinates point1(x - 1, y - 1);
            Coordinates point2(x, y - 1);
            Coordinates point3(x + 1, y - 1);
            Coordinates point4(x - 1, y);
            Coordinates point5(x + 1, y);
            Coordinates point6(x - 1, y + 1);
            Coordinates point7(x, y + 1);
            Coordinates point8(x + 1, y + 1);
            int count = 0;
            if(field.Get(point1) == Cell::Alive){
                count++;
            }
            if(field.Get(point2) == Cell::Alive){
                count++;
            }
            if(field.Get(point3) == Cell::Alive){
                count++;
            }
            if(field.Get(point4) == Cell::Alive){
                count++;
            }
            if(field.Get(point5) == Cell::Alive){
                count++;
            }
            if(field.Get(point6) == Cell::Alive){
                count++;
            }
            if(field.Get(point7) == Cell::Alive){
                count++;
            }
            if(field.Get(point8) == Cell::Alive){
                count++;
            }
            if(forLive[count] && (field.Get(point) == Cell::Alive) || forBirth[count] && (field.Get(point) == Cell::Dead)){
                field_new.Set(point, Cell::Alive);
            }
        }
    }
    std::swap(field, field_new);
}

void Model::Step(unsigned n){
    while(n--) {
        Step();
    }
}

std::string & Model::GetName() {
    return Name;
}

void Model::Save(const std::string &filename) {
    std::ofstream file(filename, std::ios::out);
    file.write("#Life 1.06\n", 11);
    file.write("#N ", 3);
    file.write(Name.c_str(), static_cast<int>(Name.length()));
    file.write("\n", 1);
    file.write("#S ", 3);
    file.write(std::to_string(size.width).c_str(), std::to_string(size.width).length());
    file.write(" ", 1);
    file.write(std::to_string(size.height).c_str(), std::to_string(size.height).length());
    file.write("\n", 1);
    file.write("#R B", 4);
    for(int i = 0; i <= 9; i++){
        if(forBirth[i]){
            file.write(std::to_string(i).c_str(), 1);
        }
    }
    file.write("/S", 2);
    for(int i = 0; i <= 9; i++){
        if(forLive[i]){
            file.write(std::to_string(i).c_str(), 1);
        }
    }
    file.write("\n", 1);
    for(int y = 0; y < size.height; y++){
        for(int x = 0; x < size.width; x++){
            Coordinates point(x, y);
            if(field.Get(point) == Cell::Alive){
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

Field & Model::GetField(){
    return field;
}
