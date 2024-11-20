#include "Controller.h"

Controller::Controller(std::string &filepath_out, std::string &filepath_config, std::vector<std::string> &arr_in, WAV & wav) :
    wav(wav),
    arr_in(arr_in),
    filepath_out(filepath_out)
{
    this->config = Configuration(filepath_config, arr_in, wav);
}

void Controller::main() {
    for(auto & creator : config.creators){
        Converter* converter = creator->factoryMethod();
        converter->Convert();
        delete converter;
        delete creator;
    }
    wav.Write(filepath_out);
}