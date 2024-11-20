#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "WAV.h"
#include <cmath>

class Converter{
public:
    std::string Name;
    short* data;
    unsigned size;
    unsigned start;

    Converter();

    virtual void Convert() = 0;

    virtual ~Converter();
};

class ConverterMute : public Converter{
public:
    unsigned end;

    ConverterMute();

    explicit ConverterMute(short * data, unsigned size, unsigned start, unsigned end);

    void Convert() override;

    ~ConverterMute() override;
};

class ConverterMix : public Converter{
public:
    short* data_add;
    unsigned size_add;

    ConverterMix();

    explicit ConverterMix(short * data, unsigned size, short * data_add, unsigned size_add, unsigned start);

    void Convert() override;

    ~ConverterMix() override;
};

class ConverterDelay : public Converter{
public:
    double dryLevel;
    double wetLevel;
    double feedback;

    ConverterDelay();

    explicit ConverterDelay(short *data, unsigned int size, unsigned int start, double dryLevel, double wetLevel, double feedback);

    void Convert() override;

    ~ConverterDelay() override;
};

class Creator{
public:
    short* data;
    unsigned size;
    unsigned start;

    Creator();

    virtual Converter* factoryMethod() = 0;

    virtual ~Creator();
};

class CreatorMute : public Creator{
public:
    unsigned end;

    CreatorMute();

    explicit CreatorMute(WAV & wav, unsigned start, unsigned end);

    Converter* factoryMethod() override;

    ~CreatorMute() override;
};

class CreatorMix : public Creator{
public:
    std::string filepath_add;
    WAV * wav_add_pointer;

    CreatorMix();

    explicit CreatorMix(WAV & wav, std::string & filepath_add, unsigned start);

    Converter* factoryMethod() override;

    ~CreatorMix() override;
};

class CreatorDelay : public Creator{
public:
    double dryLevel;
    double wetLevel;
    double feedback;

    CreatorDelay();

    explicit CreatorDelay(WAV & wav);

    explicit CreatorDelay(WAV & wav, double dryLevel, double wetLevel, double feedback);

    Converter* factoryMethod() override;

    ~CreatorDelay() override;
};