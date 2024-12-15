#include "CreatorMute.h"

#include <sstream>
#include <iostream>

CreatorMute::~CreatorMute() = default;

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

std::unique_ptr<Converter> CreatorMute::factoryMethod(unsigned second){
    return std::make_unique<ConverterMute>();
}