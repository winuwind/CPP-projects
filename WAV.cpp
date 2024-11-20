#include "WAV.h"

WAV::subChunk::subChunk() : size(0){}

WAV::subChunk_fmt::subChunk_fmt() :
    audioFormat(1),
    numChannels(1),
    sampleRate(44100),
    byteRate(88200),
    blockAlign(2),
    bitsPerSample(16){}


WAV::subChunk_data::subChunk_data() :
    data(nullptr),
    size(0){}

WAV::WAV() :
    chunkId("RIFF"),
    chunkSize(0),
    format("WAVE")
{
    chunk1 = new subChunk;
    chunk1->id = "fmt ";
    chunk2 = new subChunk;
    chunk2->id = "data";
    fmt = new subChunk_fmt;
    fmt->audioFormat = 0;   fmt->numChannels = 0;   fmt->bitsPerSample = 0;
    fmt->blockAlign = 0;    fmt->sampleRate = 0;    fmt->byteRate = 0;
    data = new subChunk_data;
    data->size = 0;
    data->data = nullptr;
}

WAV::WAV(std::string & filepath)
{
    std::ifstream file;
    file.open(filepath.c_str(), std::ios::binary);
    char str_p[4];
    auto* pointer_0 = reinterpret_cast<unsigned *>(str_p);
    auto* pointer_1 = reinterpret_cast<unsigned short *>(str_p);
    chunkSize = 0;
    file.read(str_p, 4);
    chunkId = str_p;
    file.readsome(str_p, 4);
    chunkSize = *pointer_0;
    file.readsome(str_p, 4);
    format = str_p;
    file.readsome(str_p, 4);
    chunk1 = new subChunk;
    chunk1->id = str_p;
    chunk1->size = 0;
    file.readsome(str_p, 4);
    chunk1->size = *pointer_0;
    fmt = new subChunk_fmt;
    fmt->audioFormat = 0;   fmt->numChannels = 0;   fmt->bitsPerSample = 0;
    fmt->blockAlign = 0;    fmt->sampleRate = 0;    fmt->byteRate = 0;
    file.readsome(str_p, 2);
    fmt->audioFormat = *pointer_1;
    file.readsome(str_p, 2);
    fmt->numChannels = *pointer_1;
    file.readsome(str_p, 4);
    fmt->sampleRate = *pointer_0;
    file.readsome(str_p, 4);
    fmt->byteRate = *pointer_0;
    file.readsome(str_p, 2);
    fmt->blockAlign = *pointer_1;
    file.readsome(str_p, 2);
    fmt->bitsPerSample = *pointer_1;
    chunk2 = new subChunk;
    bool flag = false;
    while(file.readsome(str_p, 4) == 4){
        chunk2->id = str_p;
        chunk2->size = 0;
        file.readsome(str_p, 4);
        chunk2->size = *pointer_0;
        if(chunk2->id.find("data") == 0){
            flag = true;
            break;
        }
        else{
            char* helper = new char[chunk2->size];
            file.readsome(helper, chunk2->size);
            delete[] helper;
        }
    }
    data = nullptr;
    if(!flag){

    }
    else {
        data = new subChunk_data;
        data->size = chunk2->size / 2;
        data->data = new short[data->size];
        char helper[2];
        auto* pointer_2 = reinterpret_cast<short *>(helper);
        for(long long i = 0; i < data->size; i++){
            file.read(helper, 2);
            data->data[i] = *pointer_2;
        }
    }
    if(chunkId.find("RIFF") || format.find("WAVE") || chunk1->id.find("fmt ")
       || chunk1->size != 16 || fmt->audioFormat != 1 || fmt->numChannels != 1
       || fmt->sampleRate != 44100 || fmt->blockAlign != 2 || fmt->bitsPerSample != 16 ||fmt->byteRate != 88200){

    }
    file.close();
}

WAV::WAV(WAV & file){
    chunkId = file.chunkId;
    chunkSize = file.chunkSize;
    format = file.format;
    chunk1 = new subChunk;
    chunk1->id = file.chunk1->id; chunk1->size = file.chunk1->size;
    chunk2 = new subChunk;
    chunk2->id = file.chunk2->id; chunk2->size = file.chunk2->size;
    fmt = new subChunk_fmt;
    fmt->audioFormat = file.fmt->audioFormat; fmt->byteRate = file.fmt->byteRate; fmt->numChannels = file.fmt->numChannels;
    fmt->sampleRate = file.fmt->sampleRate; fmt->bitsPerSample = file.fmt->bitsPerSample; fmt->blockAlign = file.fmt->blockAlign;
    data = new subChunk_data;
    data->size = file.data->size;
    data->data = new short[data->size];
    for(unsigned i = 0; i < data->size; i++){
        data->data[i] = file.data->data[i];
    }
}

WAV::~WAV(){
    delete fmt;
    delete chunk1;
    delete chunk2;
    if(data != nullptr) {
        delete[] data->data;
    }
    delete data;
}

void WAV::Write(std::string & filepath){
    std::ofstream file;
    char byte;
    unsigned x;
    file.open(filepath.c_str(), std::ios::binary);
    file.write("RIFF", 4);
    x = chunkSize;
    for(int i = 0; i < 4; i++){
        byte = x % 256;
        file.write(&byte, 1);
        x /= 256;
    }
    file.write("WAVEfmt ", 8);
    byte = 16; file.write(&byte, 1);
    byte = 0; file.write(&byte, 1); file.write(&byte, 1); file.write(&byte, 1);
    byte = 1; file.write(&byte, 1);
    byte = 0; file.write(&byte, 1);
    byte = 1; file.write(&byte, 1);
    byte = 0; file.write(&byte, 1);
    x = fmt->sampleRate;
    for(int i = 0; i < 4; i++){
        byte = x % 256;
        file.write(&byte, 1);
        x /= 256;
    }
    x = fmt->byteRate;
    for(int i = 0; i < 4; i++){
        byte = x % 256;
        file.write(&byte, 1);
        x /= 256;
    }
    byte = 2; file.write(&byte, 1);
    byte = 0; file.write(&byte, 1);
    byte = 16; file.write(&byte, 1);
    byte = 0; file.write(&byte, 1);
    file.write("data", 4);
    x = chunk2->size;
    for(int i = 0; i < 4; i++){
        byte = x % 256;
        file.write(&byte, 1);
        x /= 256;
    }
    for(unsigned i = 0; i < chunk2->size / 2; i++){
        char str[2];
        auto y = reinterpret_cast<short *>(str);
        *y = data->data[i];
        file.write(str, 2);
    }
    file.close();
}

WAV::subChunk_data* WAV::data_ret(){
    return data;
}