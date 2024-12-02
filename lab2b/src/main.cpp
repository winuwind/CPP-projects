#include "Controller.h"

#undef main

int main(int argc, char** argv) {
    std::string in;
    std::string out;
    std::string & in_ref(in);
    std::string & out_ref(out);
    in = "in.lif";
    out = "out.lif";
    unsigned long x = 0;
    if(argc > 1){
        for(int i = 1; i < argc; i++){
            std::string str(argv[i]);
            if(str == "-i") {
                if (i + 1 < argc) {
                    x = std::stoul(argv[++i]);
                }
                else{
                    std::cerr << "Not enough arguments" << std::endl;
                    Error::Parameters::print_error(argv[i]);
                }
            }
            else if(str.find("--iterations=") == 0){
                std::string x_str;
                for(int j = 13; j < str.length(); j++){
                    if (!isdigit(str[j])) {
                        Error::Parameters::print_error(argv[i]);
                        break;
                    }
                    x_str.push_back(str[j]);
                }
                x = std::stoi(x_str);
            }
            else if(str == "-f") {
                if (i + 1 < argc) {
                    in = argv[++i];
                }
                else{
                    std::cerr << "Not enough arguments" << std::endl;
                    Error::Parameters::print_error(argv[i]);
                }
            }
            else if(str.find("--input=") == 0){
                std::string str_i;
                for(int j = 8; j < str.length(); j++){
                    str_i.push_back(str[j]);
                }
                in = str_i;
            }
            else if(str == "-o") {
                if (i + 1 < argc) {
                    out = argv[++i];
                }
                else{
                    std::cerr << "Not enough arguments" << std::endl;
                    Error::Parameters::print_error(argv[i]);
                }
            }
            else if(str.find("--output=") == 0){
                std::string str_o;
                for(int j = 9; j < str.length(); j++){
                    str_o.push_back(str[j]);
                }
                out = str_o;
            }
            else if(str.find(".lif") != std::string::npos){
                in = str;
            }
            else{
                Error::Parameters::print_error(argv[i]);
            }
        }
    }
    Controller example(in_ref, out_ref, x);
    example.run();
    return 0;
}
