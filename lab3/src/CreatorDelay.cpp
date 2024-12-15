#include "CreatorDelay.h"

#include <sstream>
#include <fstream>
#include <iostream>

CreatorDelay::~CreatorDelay() = default;

CreatorDelay::CreatorDelay(std::vector<std::string> & arrIn, std::string & line):
        Creator(),
        filepath(arrIn[0]),
        dryLevel(0.7),
        wetLevel(0.3),
        feedback(0.3)
{
    std::istringstream is(line);
    std::string str;
    is >> str;
    if(!(is.fail())) {
        is >> dryLevel >> wetLevel >> feedback;
        if(is.fail()){
            std::cerr << "Bad syntax in configuration file: " << line << std::endl;
        }
        else if(dryLevel < 0 || wetLevel < 0 || feedback < 0 || dryLevel + wetLevel > 1.0 || feedback > 1.0){
            std::cerr << "Bad syntax in configuration file: " << line << std::endl;
            std::cerr << "dryLevel, wetLevel and feedback must be real numbers, 0 <= dryLevel + wetLevel <= 1, 0 <= feedback <= 1" << std::endl;
        }
    }
}

std::unique_ptr<Converter> CreatorDelay::factoryMethod(unsigned second) {
    std::ifstream file;
    file.open(filepath, std::ios::binary);
    std::vector<short> dataPrev;
    if(second != 0) {
        std::string block;
        unsigned size = 12;
        unsigned size_prev = 12;
        while (block != "data") {
            size_prev = size;
            file.seekg(size);
            char help[4];
            file.read(help, 4);
            block.clear();
            block.push_back(help[0]); block.push_back(help[1]); block.push_back(help[2]); block.push_back(help[3]);
            file.read(help, 4);
            auto *pointer = reinterpret_cast<unsigned *>(help);
            size += *pointer + 8;
        }
        size_prev += 88200 * second + 8;
        file.seekg(size_prev);
        for(int i = 0; i < 44100 && size_prev < size; i++){
            char help[2];
            file.read(help, 2);
            size_prev += 2;
            auto *pointer = reinterpret_cast<short *>(help);
            dataPrev.push_back(*pointer);
        }
    }
    file.close();
    return std::make_unique<ConverterDelay>(dataPrev, dryLevel, wetLevel, feedback);
}