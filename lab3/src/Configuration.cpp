#include "Configuration.h"

#include <sstream>
#include <fstream>
#include <iostream>

Configuration::Configuration() = default;

Configuration::Configuration(std::string & filepath, std::vector<std::string> & arr_in) {
    std::ifstream file;
    file.open(filepath.c_str(), std::ios::in);
    std::string line;
    while(std::getline(file, line)){
        std::string str;
        if(line[0] == '#'){
            continue;
        }
        std::istringstream is(line);
        is >> str;
        if(str == "mute"){
            creators.push_back(std::make_unique<CreatorMute>(line));
        }
        else if(str == "mix"){
            creators.push_back(std::make_unique<CreatorMix>(arr_in, line));
        }
        else if(str == "delay"){
            creators.push_back(std::make_unique<CreatorDelay>(arr_in, line));
        }
        else{
            std::cerr << "Error in configuration file: " << line << std::endl << R"(Command must be "mute", "mix" or "delay")" << std::endl;
            continue;
        }
    }
    file.close();
}

void Configuration::PrintSyntax() {
    std::cout << "\"#line\" - is comment that will not be read" << std::endl;
    std::cout << "\"mute <start> <end>\" - mute the interval from start to end, start and end must be non-negative integers" << std::endl;
    std::cout << R"("mix <$i> <start>" - mix with the stream from the file via the reference starting from "start", i and start must be non-negative integers, i must not be equal zero)" << std::endl;
    std::cout << R"("delay <dryLevel = 0.5> <wetLevel = 0.5> <feedback = 0.3>" - creating an echo effect, dryLevel and wetLevel are sound mixing coefficients, feedback is echo attenuation coefficient)" << std::endl;
    std::cout << "dryLevel, wetLevel and feedback must be real numbers, 0 <= dryLevel + wetLevel <= 1, 0 <= feedback <= 1" << std::endl;
}

const std::vector<std::unique_ptr<Creator>> & Configuration::GetTransformations() {
    return creators;
}