#include "ConverterDelay.h"

ConverterDelay::~ConverterDelay() = default;

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

ConverterDelay::ConverterDelay(std::vector<short> & dataPrev, double dryLevel_, double wetLevel_, double feedback_):
        dataPrev(dataPrev),
        dryLevel(dryLevel_),
        wetLevel(wetLevel_),
        feedback(feedback_)
{}