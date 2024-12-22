#include <iostream>
#include <tuple>

template <typename Ch, typename Th, typename TupleT, unsigned long long... Is>
void PrintTupleManual(std::basic_ostream<Ch, Th> & out, const TupleT & tp, std::index_sequence<Is...>) {
    unsigned long long index = 0;

    auto PrintElem = [&index, &out](const auto& x) {
        if (index++ > 0)
            out << ", ";
        out << x;
    };

    out << "(";
    (PrintElem(std::get<Is>(tp)), ...);
    out << ")";
}

template<typename Ch, typename Th, typename TupleT, std::size_t TupSize = std::tuple_size_v<TupleT>>
auto operator<<(std::basic_ostream<Ch, Th> & out, const TupleT & tp) {
    PrintTupleManual(out, tp, std::make_index_sequence<TupSize>{});
}