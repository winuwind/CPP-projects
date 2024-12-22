cmake_minimum_required(VERSION 3.29)
project(CSV_parser)

set(CMAKE_CXX_STANDARD 20)

add_executable(CSV_parser main.cpp
        PrintTuple.h
        CSVParserIterator.h
        CSVParser.h
        ParseString.h)
