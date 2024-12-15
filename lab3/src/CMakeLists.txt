cmake_minimum_required(VERSION 3.29)
project(SoundProcessor)

set(CMAKE_CXX_STANDARD 20)

add_executable(SoundProcessor
        Converters.cpp
        Converters.h
        Configuration.cpp
        Configuration.h
        Controller.cpp
        Controller.h
        main.cpp
)
