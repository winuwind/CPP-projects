#include "Converters.h"

Converter::~Converter() = default;

ConverterMute::~ConverterMute() = default;

ConverterMix::~ConverterMix() = default;

ConverterDelay::~ConverterDelay() = default;

Creator::~Creator() = default;

CreatorMute::~CreatorMute() = default;

CreatorMix::~CreatorMix() {
    delete wav_add_pointer;
}

CreatorDelay::~CreatorDelay() = default;

Converter::Converter() {
    data = nullptr;
    size = 0;
    start = 0;
}

ConverterMute::ConverterMute() {
    Name = "Mute";
    data = nullptr;
    size = 0;
    start = 0;
    end = 0;
}

ConverterMute::ConverterMute(short* data, unsigned size, unsigned int start, unsigned int end) {
    Name = "Mute";
    this->data = data;
    this->size = size;
    this->start = start;
    this->end = end;
}

void ConverterMute::Convert(){
    unsigned frequency = 44100;
    start *= frequency; end *= frequency;
    if(start >= size){
        return;
    }
    for(unsigned i = start; i < size && i < end; i++){
        data[i] = 0;
    }
}

ConverterMix::ConverterMix() {
    Name = "Mix";
    data = nullptr;
    size = 0;
    start = 0;
    data_add = nullptr;
    size_add = 0;
}

ConverterMix::ConverterMix(short *data, unsigned int size, short *data_add, unsigned int size_add,unsigned int start) {
    this->data = data;
    this->size = size;
    this->data_add = data_add;
    this->size_add = size_add;
    this->start = start;
    Name = "Mix";
}

void ConverterMix::Convert(){
    unsigned frequency = 44100;
    start *= frequency;
    short data1 = 0, data2 = 0;
    for(unsigned i = start; i < size && i < size_add; i++){
        data1 = data[i]; data2 = data_add[i];
        data[i] = data1 / 2 + data2 / 2 + (data1 % 2 + data2 % 2) / 2;
    }
}

ConverterDelay::ConverterDelay() {
    data = nullptr;
    size = 0;
    start = 0;
    Name = "Delay";
    dryLevel = 0.5;
    wetLevel = 0.5;
    feedback = 0.3;
}

ConverterDelay::ConverterDelay(short *data, unsigned int size, unsigned int start, double dryLevel, double wetLevel, double feedback) {
    this->data = data;
    this->size = size;
    Name = "Delay";
    this->start = start;
    this->dryLevel = dryLevel;
    this->wetLevel = wetLevel;
    this->feedback = feedback;
}

void ConverterDelay::Convert() {
    std::vector<short> buffer(start, 0);
    for(unsigned i = 0; i < start; i++){
        buffer[i] = data[i];
    }
    unsigned index = 0;
    for(unsigned i = start; i < size; i++){
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
        index = (index + 1) % start;
    }
}

Creator::Creator() {
    data = nullptr;
    size = 0;
    start = 0;
}

CreatorMute::CreatorMute() {
    data = nullptr;
    size = 0;
    start = 0;
    end = 0;
}

CreatorMute::CreatorMute(WAV &wav, unsigned int start, unsigned int end) {
    data = wav.data_ret()->data;
    size = wav.data_ret()->size;
    this->start = start;
    this->end = end;
}

CreatorMix::CreatorMix() {
    data = nullptr;
    size = 0;
    start = 0;
    wav_add_pointer = nullptr;
}

CreatorMix::CreatorMix(WAV &wav, std::string & filepath_add, unsigned int start) {
    data = wav.data_ret()->data;
    size = wav.data_ret()->size;
    this->filepath_add = filepath_add;
    this->start = start;
    wav_add_pointer = nullptr;
}

CreatorDelay::CreatorDelay() {
    data = nullptr;
    size = 0;
    start = 0;
    dryLevel = 0.5;
    wetLevel = 0.5;
    feedback = 0.3;
}

CreatorDelay::CreatorDelay(WAV &wav) {
    data = wav.data_ret()->data;
    size = wav.data_ret()->size;
    this->start = 2205;
    dryLevel = 0.5;
    wetLevel = 0.5;
    feedback = 0.3;
}

CreatorDelay::CreatorDelay(WAV &wav, double dryLevel, double wetLevel, double feedback) {
    data = wav.data_ret()->data;
    size = wav.data_ret()->size;
    this->start = 2205;
    this->dryLevel = dryLevel;
    this->wetLevel = wetLevel;
    this->feedback = feedback;
}

Converter* CreatorMute::factoryMethod(){
    return new ConverterMute(data, size, start, end);
}

Converter* CreatorMix::factoryMethod() {
    wav_add_pointer = new WAV(filepath_add);
    return new ConverterMix(data, size, wav_add_pointer->data_ret()->data, wav_add_pointer->data_ret()->size, start);
}

Converter *CreatorDelay::factoryMethod() {
    return new ConverterDelay(data, size, start, dryLevel, wetLevel, feedback);
}
