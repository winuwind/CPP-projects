#include "ConverterMix.h"

ConverterMix::~ConverterMix() = default;

std::vector<short> & ConverterMix::Convert(std::vector<short> & data){
    short data1 = 0, data2 = 0;
    for(unsigned i = 0; i < data.size() && i < dataAdd.size(); i++){
        data1 = data[i]; data2 = dataAdd[i];
        data[i] = data1 / 2 + data2 / 2 + (data1 % 2 + data2 % 2) / 2;
    }
    return data;
}

ConverterMix::ConverterMix(std::vector<short> & dataAdd_):
        dataAdd(dataAdd_)
{}