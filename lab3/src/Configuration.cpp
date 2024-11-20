#include "Configuration.h"

Configuration::Configuration() = default;

Configuration::Configuration(std::string & filepath, std::vector<std::string> & arr_in, WAV & wav) {
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
            unsigned start = 0, end = 0;
            is >> start >> end;
            if(is.fail()){
                std::cerr << "bad syntax in configuration file: " << line << std::endl;
                continue;
            }
            Creator* creator_p = new CreatorMute(wav, start, end);
            creators.push_back(creator_p);
        }
        else if(str == "mix"){
            is >> str;
            if(str[0] == '$'){
                str[0] = '0';
            }
            else{
                std::cerr << "Bad syntax in configuration file: " << line << std::endl;
                std::cerr << "First argument must be a reference to the input file" << std::endl;
                continue;
            }
            bool flag = false;
            for(size_t i = 0; i < str.length(); i++){
                if(!std::isdigit(str[i])){
                    flag = true;
                    break;
                }
            }
            if(flag){
                std::cerr << "Bad syntax in configuration file: " << line << std::endl;
                continue;
            }
            int index = std::stoi(str);
            if(index < 1 || index > arr_in.size()){
                std::cerr << "Bad syntax in configuration file: " << line << std::endl;
                std::cerr << "$" << index << " must be less than number of input files (" << arr_in.size() << "), but greater then zero" << std::endl;
                continue;
            }
            unsigned start = 0;
            is >> start;
            if(is.fail()){
                std::cerr << "Bad syntax in configuration file: " << line << std::endl;
                continue;
            }
            Creator* creator_p = new CreatorMix(wav, arr_in[index - 1], start);
            creators.push_back(creator_p);
        }
        else if(str == "delay"){
            if(is.eof()){
                Creator* creator_p = new CreatorDelay(wav);
                creators.push_back(creator_p);
            }
            else{
                double dryLevel = 0.5;
                double wetLevel = 0.5;
                double feedback = 0.3;
                is >> dryLevel >> wetLevel >> feedback;
                if(is.fail()){
                    std::cerr << "Bad syntax in configuration file: " << line << std::endl;
                    continue;
                }
                else if(dryLevel < 0 || wetLevel < 0 || feedback < 0 || dryLevel + wetLevel > 1.0 || feedback > 1.0){
                    std::cerr << "Bad syntax in configuration file: " << line << std::endl;
                    std::cerr << "dryLevel, wetLevel and feedback must be real numbers, 0 <= dryLevel + wetLevel <= 1, 0 <= feedback <= 1" << std::endl;
                    continue;
                }
                Creator* creator_p = new CreatorDelay(wav, dryLevel, wetLevel, feedback);
                creators.push_back(creator_p);
            }
        }
        else{

            continue;
        }
    }
    file.close();
}

void Configuration::PrintSyntax() {
    std::cout << "\"#line\" - is comment that will not be read" << std::endl;
    std::cout << "\"mute start end\" - mute the interval from start to end, start and end must be non-negative integers" << std::endl;
    std::cout << R"("mix $i start" - mix with the stream from the file via the reference starting from "start", i and start must be non-negative integers, i must not be equal zero)" << std::endl;
    std::cout << R"("delay <dryLevel = 0.5> <wetLevel = 0.5> <feedback = 0.3>" - creating an echo effect, dryLevel and wetLevel are sound mixing coefficients, feedback is echo attenuation coefficient)" << std::endl;
    std::cout << "dryLevel, wetLevel and feedback must be real numbers, 0 <= dryLevel + wetLevel <= 1, 0 <= feedback <= 1" << std::endl;
}