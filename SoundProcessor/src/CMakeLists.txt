cmake_minimum_required(VERSION 3.29)
project(SoundProcessor)

set(CMAKE_CXX_STANDARD 20)

add_executable(SoundProcessor
        Configuration.cpp
        Configuration.h
        Controller.cpp
        Controller.h
        main.cpp
        Converter.h
        ConverterMute.cpp
        ConverterMute.h
        ConverterMix.cpp
        ConverterMix.h
        ConverterDelay.cpp
        ConverterDelay.h
        Creator.cpp
        Creator.h
        CreatorMute.cpp
        CreatorMute.h
        CreatorMix.cpp
        CreatorMix.h
        CreatorDelay.cpp
        CreatorDelay.h
)
