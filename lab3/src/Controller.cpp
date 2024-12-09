#include "Controller.h"

Controller::Controller(std::string &filepath_out, std::string &filepath_config, std::vector<std::string> &arr_in) :
        arr_in(arr_in),
        filepath_out(filepath_out)
{
    wav = WAV(arr_in[0]);
    config = Configuration(filepath_config, arr_in, wav);
}

void Controller::main() {
    std::vector<std::unique_ptr<Creator>> & creators = config.GetTransformations();
    for(auto & creator : creators){
        std::unique_ptr<Converter> converter = creator->factoryMethod();
        converter->Convert();
    }
    wav.Write(filepath_out);
}
