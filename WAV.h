#pragma once

#include <iostream>
#include <fstream>

class WAV{
private:
    std::string chunkId;
    unsigned int chunkSize;
    std::string format;
    struct subChunk{
        unsigned int size;
        std::string id;
        subChunk();
    };
    subChunk* chunk1;
    subChunk* chunk2;
    struct subChunk_fmt {
        unsigned short audioFormat;
        unsigned short numChannels;
        unsigned int sampleRate;
        unsigned int byteRate;
        unsigned short blockAlign;
        unsigned short bitsPerSample;
        subChunk_fmt();
    };
    subChunk_fmt* fmt;
    struct subChunk_data{
        short* data;
        unsigned size;
        subChunk_data();
    };
    subChunk_data* data;
public:
    WAV();

    explicit WAV(std::string & filepath);

    WAV(WAV & file);

    ~WAV();

    void Write(std::string & filepath);

    subChunk_data* data_ret();
};