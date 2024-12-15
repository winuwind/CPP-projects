#include "Converters.h"

#include <sstream>
#include <iostream>
#include <fstream>

Converter::~Converter() = default;

ConverterMute::~ConverterMute() = default;

ConverterMix::~ConverterMix() = default;

ConverterDelay::~ConverterDelay() = default;

Creator::~Creator() = default;

CreatorMute::~CreatorMute() = default;

CreatorMix::~CreatorMix() = default;

CreatorDelay::~CreatorDelay() = default;

std::vector<short> & ConverterMute::Convert(std::vector<short> & data){
    for(short & i : data){
        i = 0;
    }
    return data;
}

std::vector<short> & ConverterMix::Convert(std::vector<short> & data){
    short data1 = 0, data2 = 0;
    for(unsigned i = 0; i < data.size() && i < dataAdd.size(); i++){
        data1 = data[i]; data2 = dataAdd[i];
        data[i] = data1 / 2 + data2 / 2 + (data1 % 2 + data2 % 2) / 2;
    }
    return data;
}

std::vector<short> & ConverterDelay::Convert(std::vector<short> & data) {
    unsigned delay = 2205;
    unsigned start = 0;
    std::vector<short> buffer(delay);
    for(unsigned i = 1; i <= delay && i <= dataPrev.size(); i++){
        buffer[delay - i] = dataPrev[dataPrev.size() - i];
    }
    if(dataPrev.empty()){
        for(unsigned i = 0; i < delay && i < data.size(); i++){
            buffer[i] = data[i];
        }
        start = delay;
    }
    unsigned index = 0;
    for(unsigned i = start; i < data.size(); i++){
        double data_1 = data[i], data_2 = buffer[index];
        data[i] = static_cast<short>(data_1 * dryLevel + data_2 * wetLevel);
        data_1 += feedback * buffer[index];
        if(data_1 > 32767){
            data_1 = 32767;
        }
        else if(data_1 < -32768){
            data_1 = -32768;
        }
        buffer[index] = static_cast<short>(data_1);
        index = (index + 1) % delay;
    }
    return data;
}

ConverterMute::ConverterMute() = default;

ConverterMix::ConverterMix(std::vector<short> & dataAdd_):
        dataAdd(dataAdd_)
{}

ConverterDelay::ConverterDelay(std::vector<short> & dataPrev, double dryLevel_, double wetLevel_, double feedback_):
        dataPrev(dataPrev),
        dryLevel(dryLevel_),
        wetLevel(wetLevel_),
        feedback(feedback_)
{}

Creator::Creator():
        start(0),
        end(0)
{}

void Creator::setData(unsigned int start_, unsigned int end_) {
    start = start_;
    end = end_;
}

unsigned Creator::getStart() const {
    return start;
}

unsigned Creator::getEnd() const {
    return end;
}

CreatorMute::CreatorMute(std::string & line):
        Creator()
{
    std::istringstream is(line);
    std::string str;
    is >> str;
    unsigned start;
    unsigned end;
    is >> start;
    if(is.fail() || is.eof()){
        start = 0;
        end = 0;
        std::cerr << "Bad syntax in configuration file: " << line << std::endl;
    }
    if(!is.fail() && !is.eof()){
        is >> end;
        if(is.fail()){
            end = 0;
            std::cerr << "Bad syntax in configuration file: " << line << std::endl;
        }
    }
    setData(start, end);
}

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

std::unique_ptr<Converter> CreatorMute::factoryMethod(unsigned second){
    return std::make_unique<ConverterMute>();
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