#include "CreatorMix.h"

#include <sstream>
#include <fstream>
#include <iostream>

CreatorMix::~CreatorMix() = default;

CreatorMix::CreatorMix(std::vector<std::string> & arrIn, std::string & line):
        Creator()
{
    std::istringstream is(line);
    std::string str;
    filepathInAdd = arrIn[0];
    unsigned start = 0;
    is >> str;
    is >> str;
    if(!(is.fail())){
        bool flag = false;
        if (str[0] == '$') {
            str[0] = '0';
        }
        else{
            flag = true;
            std::cerr << "First argument must be a reference to the input file" << std::endl;
        }
        for(size_t i = 0; i < str.length(); i++){
            if(!std::isdigit(str[i])){
                flag = true;
                break;
            }
        }
        if(!flag){
            int index = std::stoi(str);
            if(index < 1 || index > arrIn.size()){
                std::cerr << "$" << index << " must be less than number of input files (" << arrIn.size() << "), but greater then zero" << std::endl;
            }
            else{
                filepathInAdd = arrIn[index - 1];
                is >> start;
                if(is.fail()){
                    std::cerr << "Bad syntax in configuration file: " << line << std::endl;
                    start = 0;
                }
            }
        }
        else{
            std::cerr << "Bad syntax in configuration file: " << line << std::endl;
        }
    }
    setData(start, 0);
}

std::unique_ptr<Converter> CreatorMix::factoryMethod(unsigned second) {
    second -= getStart();
    std::ifstream file;
    file.open(filepathInAdd, std::ios::binary);
    std::vector<short> dataAdd;
    std::string block;
    unsigned size = 12;
    unsigned size_prev = 12;
    while(block != "data") {
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
        dataAdd.push_back(*pointer);
    }
    file.close();
    return std::make_unique<ConverterMix>(dataAdd);
}