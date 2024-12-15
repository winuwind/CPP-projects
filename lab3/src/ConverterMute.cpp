#include "ConverterMute.h"


ConverterMute::~ConverterMute() = default;

std::vector<short> & ConverterMute::Convert(std::vector<short> & data){
    for(short & i : data){
        i = 0;
    }
    return data;
}

ConverterMute::ConverterMute() = default;