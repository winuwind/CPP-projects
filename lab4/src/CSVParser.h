#pragma once

#include "CSVParserIterator.h"
#include "ParseString.h"

#include <fstream>
#include <tuple>

template<typename ...Types> class Iterator;

struct Settings{
    char delimLines_;
    char delimCells_;
    char escapeSymbol_;

    Settings();

    Settings(char delimLines, char delimCells, char escapeSymbol);

    Settings(Settings & other);
};

template<typename ...Types> class CSVParser{
public:
    CSVParser(std::ifstream & file, unsigned count_skip, Settings & settings);

    Iterator<Types...> begin();

    Iterator<Types...> end();
private:
    friend Iterator<Types...>;
    std::ifstream file_;
    unsigned minPosition_;
    unsigned position_;
    Settings settings_;
    std::string lastLine_;

    void ReadLine();
};

//CSPParser.cpp
Settings::Settings() {
    delimLines_ = '\n';
    delimCells_ = ',';
    escapeSymbol_ = '\"';
}

Settings::Settings(char delimLines, char delimCells, char escapeSymbol) {
    delimLines_ = delimLines;
    delimCells_ = delimCells;
    escapeSymbol_ = escapeSymbol;
}

Settings::Settings(Settings & other) {
    delimLines_ = other.delimLines_;
    delimCells_ = other.delimCells_;
    escapeSymbol_ = other.escapeSymbol_;
}

template<typename ...Types> CSVParser<Types...>::CSVParser(std::ifstream & file, unsigned int count_skip, Settings & settings) :
        settings_(settings)
{
    file_ = std::move(file);
    position_ = 0;
    while(count_skip--){
        std::getline(file_, lastLine_, settings_.delimLines_);
        position_ += lastLine_.length();
    }
    minPosition_ = position_;
}

template<typename ...Types> Iterator<Types...> CSVParser<Types...>::begin(){
    return Iterator<Types...>(*this, State::Start);
}

template<typename ...Types> Iterator<Types...> CSVParser<Types...>::end(){
    return Iterator<Types...>(*this, State::End);
}

template<typename ...Types> void CSVParser<Types...>::ReadLine(){
    std::getline(file_, lastLine_, settings_.delimLines_);
    position_ += lastLine_.length();
    while(!lastLine_.empty() && lastLine_[lastLine_.length() - 1] == settings_.escapeSymbol_){
        lastLine_[lastLine_.length() - 1] = settings_.delimLines_;
        std::string newLine;
        std::getline(file_, newLine, settings_.delimLines_);
        lastLine_ += newLine;
        position_ += newLine.length();
    }
}
