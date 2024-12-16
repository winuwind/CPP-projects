#pragma once

#include <sstream>
#include <tuple>

template<typename TupleT, unsigned long long... Is>
TupleT ParseStringManual(TupleT & tuple, std::string & str, char delim, std::index_sequence<Is...>){
    std::istringstream isStr(str);

    auto Func = [&isStr, delim](auto & token){
        std::string elem;
        std::getline(isStr, elem, delim);
        std::istringstream is(elem);
        is >> token;
    };

    (Func(std::get<Is>(tuple)), ...);
    return tuple;
}


template<typename TupleT, std::size_t TupSize = std::tuple_size_v<TupleT>>
TupleT ParseString(TupleT & tuple_, std::string & str, char delim) {
    return ParseStringManual(tuple_, str, delim, std::make_index_sequence<TupSize>{});
}