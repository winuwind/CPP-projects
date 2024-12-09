#include "Converters.h"

#include <vector>

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

Converter::Converter(std::string & Name, short* data, unsigned size, unsigned start) {
    this->Name = Name;
    this->data = data;
    this->size = size;
    this->start = start;
}

short* Converter::GetData() const {
    return data;
}

unsigned Converter::GetSize() const {
    return size;
}

unsigned Converter::GetStart() const {
    return start;
}

short* Creator::GetData() const {
    return data;
}

unsigned Creator::GetSize() const {
    return size;
}

unsigned Creator::GetStart() const {
    return start;
}

ConverterMute::ConverterMute() :
        Converter((std::string &) "Mute", nullptr, 0, 0) {
    end = 0;
}

ConverterMute::ConverterMute(short* data, unsigned size, unsigned int start, unsigned int end) :
        Converter((std::string &) "Mute", data, size, start) {
    this->end = end;
}

void ConverterMute::Convert(){
    short* data = GetData();
    unsigned size = GetSize();
    unsigned start = GetStart();
    unsigned frequency = 44100;
    start *= frequency; end *= frequency;
    if(start >= size){
        return;
    }
    for(unsigned i = start; i < size && i < end; i++){
        data[i] = 0;
    }
}

ConverterMix::ConverterMix() :
        Converter((std::string &) "Mix", nullptr, 0, 0){
    data_add = nullptr;
    size_add = 0;
}

ConverterMix::ConverterMix(short *data, unsigned int size, short *data_add, unsigned int size_add,unsigned int start) :
        Converter((std::string &) "Mix", data, size, start){
    this->data_add = data_add;
    this->size_add = size_add;
}

void ConverterMix::Convert(){
    short* data = GetData();
    unsigned size = GetSize();
    unsigned start = GetStart();
    unsigned frequency = 44100;
    start *= frequency;
    short data1 = 0, data2 = 0;
    for(unsigned i = start; i < size && i < size_add; i++){
        data1 = data[i]; data2 = data_add[i];
        data[i] = data1 / 2 + data2 / 2 + (data1 % 2 + data2 % 2) / 2;
    }
}

ConverterDelay::ConverterDelay() :
        Converter((std::string &) "Delay", nullptr, 0, 0){
    dryLevel = 0.5;
    wetLevel = 0.5;
    feedback = 0.3;
}

ConverterDelay::ConverterDelay(short *data, unsigned int size, unsigned int start, double dryLevel, double wetLevel, double feedback) :
        Converter((std::string &) "Delay", data, size, start){
    this->dryLevel = dryLevel;
    this->wetLevel = wetLevel;
    this->feedback = feedback;
}

void ConverterDelay::Convert() {
    short* data = GetData();
    unsigned size = GetSize();
    unsigned start = GetStart();
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

Creator::Creator(short* data, unsigned size, unsigned start) {
    this->data = data;
    this->size = size;
    this->start = start;
}

CreatorMute::CreatorMute() :
        Creator(nullptr, 0, 0){
    end = 0;
}

CreatorMute::CreatorMute(WAV &wav, unsigned int start, unsigned int end) :
        Creator(wav.data_ret()->data, wav.data_ret()->size, start){
    this->end = end;
}

CreatorMix::CreatorMix() :
        Creator(nullptr, 0, 0){
    wav_add_pointer = nullptr;
}

CreatorMix::CreatorMix(WAV &wav, std::string & filepath_add, unsigned int start) :
        Creator(wav.data_ret()->data, wav.data_ret()->size, start){
    this->filepath_add = filepath_add;
    wav_add_pointer = nullptr;
}

CreatorDelay::CreatorDelay() :
        Creator(nullptr, 0, 0){
    dryLevel = 0.5;
    wetLevel = 0.5;
    feedback = 0.3;
}

CreatorDelay::CreatorDelay(WAV &wav) :
        Creator(wav.data_ret()->data, wav.data_ret()->size, 2205){
    dryLevel = 0.5;
    wetLevel = 0.5;
    feedback = 0.3;
}

CreatorDelay::CreatorDelay(WAV &wav, double dryLevel, double wetLevel, double feedback) :
        Creator(wav.data_ret()->data, wav.data_ret()->size, 2205){
    this->dryLevel = dryLevel;
    this->wetLevel = wetLevel;
    this->feedback = feedback;
}

std::unique_ptr<Converter> CreatorMute::factoryMethod(){
    short* data = GetData();
    unsigned size = GetSize();
    unsigned start = GetStart();
    return std::make_unique<ConverterMute>(data, size, start, end);
}

std::unique_ptr<Converter> CreatorMix::factoryMethod() {
    short* data = GetData();
    unsigned size = GetSize();
    unsigned start = GetStart();
    wav_add_pointer = new WAV(filepath_add);
    return std::make_unique<ConverterMix>(data, size, wav_add_pointer->data_ret()->data, wav_add_pointer->data_ret()->size, start);
}

std::unique_ptr<Converter> CreatorDelay::factoryMethod() {
    short* data = GetData();
    unsigned size = GetSize();
    unsigned start = GetStart();
    return std::make_unique<ConverterDelay>(data, size, start, dryLevel, wetLevel, feedback);
}
