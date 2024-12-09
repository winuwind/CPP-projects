#pragma once

#include <string>
#include "WAV.h"
#include <memory>

class Converter{
private:
    std::string Name;
    short* data;
    unsigned size;
    unsigned start;
protected:
    Converter(std::string & Name, short* data, unsigned size, unsigned start);
public:
    virtual void Convert() = 0;

    virtual ~Converter();

    [[nodiscard]] short* GetData() const;

    [[nodiscard]] unsigned GetSize() const;

    [[nodiscard]] unsigned GetStart() const;
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
private:
    short* data_add;
    unsigned size_add;
public:
    ConverterMix();

    explicit ConverterMix(short * data, unsigned size, short * data_add, unsigned size_add, unsigned start);

    void Convert() override;

    ~ConverterMix() override;
};

class ConverterDelay : public Converter{
private:
    double dryLevel;
    double wetLevel;
    double feedback;
public:
    ConverterDelay();

    explicit ConverterDelay(short *data, unsigned int size, unsigned int start, double dryLevel, double wetLevel, double feedback);

    void Convert() override;

    ~ConverterDelay() override;
};

class Creator{
private:
    short* data;
    unsigned size;
    unsigned start;
protected:
    Creator(short* data, unsigned size, unsigned start);
public:
    virtual std::unique_ptr<Converter> factoryMethod() = 0;

    virtual ~Creator();

    [[nodiscard]] short* GetData() const;

    [[nodiscard]] unsigned GetSize() const;

    [[nodiscard]] unsigned GetStart() const;
};

class CreatorMute : public Creator{
private:
    unsigned end;
public:
    CreatorMute();

    explicit CreatorMute(WAV & wav, unsigned start, unsigned end);

    std::unique_ptr<Converter> factoryMethod() override;

    ~CreatorMute() override;
};

class CreatorMix : public Creator{
private:
    std::string filepath_add;
    WAV * wav_add_pointer;
public:
    CreatorMix();

    explicit CreatorMix(WAV & wav, std::string & filepath_add, unsigned start);

    std::unique_ptr<Converter> factoryMethod() override;

    ~CreatorMix() override;
};

class CreatorDelay : public Creator{
private:
    double dryLevel;
    double wetLevel;
    double feedback;
public:
    CreatorDelay();

    explicit CreatorDelay(WAV & wav);

    explicit CreatorDelay(WAV & wav, double dryLevel, double wetLevel, double feedback);

    std::unique_ptr<Converter> factoryMethod() override;

    ~CreatorDelay() override;
};
