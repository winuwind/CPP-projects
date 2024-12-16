#pragma once

#include "CSVParser.h"
#include "ParseString.h"

#include <tuple>
#include <sstream>

template<typename ...Types> class CSVParser;

enum class State{
    Start,
    Normal,
    End
};

template<typename ...Types>
class Iterator{
public:
    std::tuple<Types...> & operator*();

    Iterator & operator++();

    bool operator!=(const Iterator & other);
private:
    friend CSVParser<Types...>;
    CSVParser<Types...> & parser_;
    std::tuple<Types...> tuple_;
    State state_;

    Iterator(CSVParser<Types...> & parser, State state);
};

template<typename ...Types> Iterator<Types...>::Iterator(CSVParser<Types...> & parser, State state) :
        parser_(parser),
        state_(state){
    if(state == State::Start){
        parser_.file_.seekg(parser_.minPosition_);
        state_ = State::Normal;
        parser_.position_ = parser_.minPosition_;
        parser_.ReadLine();
    }
}

template<typename ...Types> Iterator<Types...> & Iterator<Types...>::operator++() {
    if(state_ == State::End) {
        return *this;
    }
    parser_.ReadLine();
    if (parser_.file_.eof()) {
        state_ = State::End;
        return *this;
    }
    return *this;
}

template<typename ...Types> std::tuple<Types...> & Iterator<Types...>::operator*(){
    tuple_ = ParseString(tuple_, parser_.lastLine_, parser_.settings_.delimCells_);
    return tuple_;
}

template<typename ...Types> bool Iterator<Types...>::operator!=(const Iterator<Types...> & other) {
    return (state_ != other.state_) || (parser_.position_ != other.parser_.position_);
}