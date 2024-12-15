#pragma once

#include <vector>
#include <memory>

class Converter{
public:
    virtual std::vector<short> & Convert(std::vector<short> & data) = 0;

    virtual ~Converter();
};

class ConverterMute : public Converter{
public:
    explicit ConverterMute();

    std::vector<short> & Convert(std::vector<short> & data) override;

    ~ConverterMute() override;
};

class ConverterMix : public Converter{
private:
    std::vector<short> dataAdd;
public:
    explicit ConverterMix(std::vector<short> & dataAdd_);

    std::vector<short> & Convert(std::vector<short> & data) override;

    ~ConverterMix() override;
};

class ConverterDelay : public Converter{
private:
    std::vector<short> dataPrev;
    double dryLevel;
    double wetLevel;
    double feedback;
public:
    ConverterDelay(std::vector<short> & dataPrev_, double dryLevel_, double wetLevel_, double feedback_);

    std::vector<short> & Convert(std::vector<short> & data) override;

    ~ConverterDelay() override;
};

class Creator{
private:
    unsigned start;
    unsigned end;
protected:
    Creator();
public:
    virtual std::unique_ptr<Converter> factoryMethod(unsigned second) = 0;

    virtual ~Creator();

    void setData(unsigned start_, unsigned end_);

    [[nodiscard]] unsigned getStart() const;

    [[nodiscard]] unsigned getEnd() const;
};

class CreatorMute : public Creator{
public:
    explicit CreatorMute(std::string & line);

    std::unique_ptr<Converter> factoryMethod(unsigned second) override;

    ~CreatorMute() override;
};

class CreatorMix : public Creator{
private:
    std::string filepathInAdd;
public:
    CreatorMix(std::vector<std::string> & arrIn, std::string & line);

    std::unique_ptr<Converter> factoryMethod(unsigned second) override;

    ~CreatorMix() override;
};

class CreatorDelay : public Creator{
private:
    std::string filepath;
    double dryLevel;
    double wetLevel;
    double feedback;
public:
    CreatorDelay(std::vector<std::string> & arrIn,std::string & line);

    std::unique_ptr<Converter> factoryMethod(unsigned second) override;

    ~CreatorDelay() override;
};