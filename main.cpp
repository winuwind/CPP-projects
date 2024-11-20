#include "Controller.h"

int main(int argc, char** argv){
    std::string out, config;
    std::vector<std::string> arr_in;
    for(int i = 1; i < argc; i++){
        std::string arg = argv[i];
        if(arg == "-c"){
            if(!config.empty()){
                std::cerr << "Too many keys \"-c\"" << std::endl;
                return 0;
            }
            if(i + 1 < argc){
                config = argv[++i];
            }
        }
        else if(arg == "-h"){
            Configuration::PrintSyntax();
        }
        else if(out.empty()){
            out = arg;
        }
        else{
            arr_in.push_back(arg);
        }
    }
    WAV wav(arr_in[0]);
    Controller controller(out, config, arr_in, wav);
    controller.main();
    return 0;
}