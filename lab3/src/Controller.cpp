#include "Controller.h"

#include <fstream>

Controller::Controller(std::string &filepath_out, std::string &filepath_config, std::vector<std::string> &arr_in) :
        arr_in(arr_in),
        filepath_out(filepath_out)
{
    config = Configuration(filepath_config, arr_in);
}

void Controller::body() {
    const std::vector<std::unique_ptr<Creator>> & creators = config.GetTransformations();
    std::ifstream fileIn;
    fileIn.open(arr_in[0], std::ios::binary);
    std::ofstream fileOut;
    fileOut.open(filepath_out, std::ios::binary);
    char help[12];
    fileIn.read(help, 12);
    fileOut.write(help, 12);
    unsigned size = 0;
    std::string block;
    while(block != "data"){
        if(size) {
            char* data = new char[size];
            fileIn.read(data, size);
            fileOut.write(block.c_str(), 4);
            char help_new[4];
            auto *pointer = reinterpret_cast<unsigned*>(help_new);
            *pointer = size;
            fileOut.write(help_new, 4);
            fileOut.write(data, size);
            delete[] data;
        }
        char help_new[4];
        fileIn.read(help_new, 4);
        block.clear();
        block.push_back(help_new[0]); block.push_back(help_new[1]); block.push_back(help_new[2]); block.push_back(help_new[3]);
        fileIn.read(help_new, 4);
        auto *pointer = reinterpret_cast<unsigned*>(help_new);
        size = *pointer;
    }
    fileOut.write(block.c_str(), 4);
    char help_[4];
    auto *pointer_ = reinterpret_cast<unsigned*>(help_);
    *pointer_ = size;
    fileOut.write(help_, 4);
    for(unsigned i = 0; i < size; i += 88200){
        std::vector<short> data_;
        for(unsigned j = 0; j < 88200 && i + j < size; j += 2){
            char help_new[2];
            fileIn.read(help_new, 2);
            auto* pointer = reinterpret_cast<short *>(help_new);
            data_.push_back(*pointer);
        }
        for(const auto & creator : creators){
            if(i / 88200 >= creator->getStart() && (i / 88200 <= creator->getEnd() || creator->getEnd() == 0) || creator->getStart() == 0 && creator->getEnd() == 0){
                std::unique_ptr<Converter> converter = creator->factoryMethod(i / 88200);
                data_ = converter->Convert(data_);
            }
        }
        for(short j : data_){
            char str[2];
            auto y = reinterpret_cast<short *>(str);
            *y = j;
            fileOut.write(str, 2);
        }
    }
    fileIn.close();
    fileOut.close();
}