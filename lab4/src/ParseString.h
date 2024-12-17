#pragma once

#include <sstream>
#include <tuple>

template<typename TupleT, unsigned long long... Is>
TupleT ParseStringManual(TupleT & tuple, std::string & str, char delim, char escapeSymbol, std::index_sequence<Is...>){
    std::istringstream isStr(str);

    auto Func = [&isStr, delim, escapeSymbol](auto & token){
        std::string elem;
        std::getline(isStr, elem, delim);
        while(!elem.empty() && elem[elem.size() - 1] == escapeSymbol){
            elem[elem.size() - 1] = delim;
            std::string nextElem;
            std::getline(isStr, nextElem, delim);
            elem += nextElem;

        }
        std::istringstream is(elem);
        is >> token;
    };

    (Func(std::get<Is>(tuple)), ...);
    return tuple;
}


template<typename TupleT, std::size_t TupSize = std::tuple_size_v<TupleT>>
TupleT ParseString(TupleT & tuple_, std::string & str, char delim, char escapeSymbol) {
    return ParseStringManual(tuple_, str, delim, escapeSymbol, std::make_index_sequence<TupSize>{});
}
