#include <iostream>
#include <fstream>
#include <string>
#include <map>


bool CheckSymbol(char symbol){
    return !(std::isdigit(symbol) || std::isalpha(symbol));
}


struct Cmp {
    bool operator()(const std::pair<std::string, int> & p1, const std::pair<std::string, int> & p2) const {
        if(p1.second != p2.second){
            return p1.second < p2.second;
        }
        return true;
    }
};


int main(int args, char** argv) {
    if(args < 3){
        std::cout << "Error args" << std::endl;
        return 0;
    }

    std::ifstream input;
    input.open(argv[1], std::ios::in);
    if(!input.is_open()){
        std::cout << "Error in input file" << std::endl;
        return 0;
    }

    std::ofstream out;
    out.open(argv[2], std::ios::out);
    if(!input.is_open()){
        std::cout << "Error in out file" << std::endl;
        return 0;
    }

    std::string word;
    std::string some_word;
    std::string first_line = "word, frequency, frequency(%)\n";
    std::map<std::string, int> dict;
    int count_word = 0;


    while(std::getline(input, some_word, ' ')){
        for(int i = 0; i < some_word.length(); i++){
            char symbol = some_word[i];
            if(isalpha(symbol) && symbol < 'a'){
                symbol += 'a' - 'A';
            }
            if(CheckSymbol(symbol) && !word.empty()){
                auto it = dict.insert({word, 1});
                if(!it.second){
                    it.first->second++;
                }
                count_word++;
                word.clear();
            }
            else if(!CheckSymbol(symbol)){
                word += symbol;
            }
        }
        if(!word.empty()){
            auto it = dict.insert({word, 1});
            if(!it.second){
                it.first->second++;
            }
            count_word++;
            word.clear();
        }
    }


    std::multimap<int, std::string> dict_help;
    for(auto it = dict.begin(); it != dict.end(); it++){
        dict_help.insert({it->second, it->first});
    }


    out.write(first_line.c_str(), first_line.length());
    for(auto it = dict_help.rbegin(); it != dict_help.rend(); it++) {
        std::string line(it->second); line.append(", ");
        line.append(std::to_string(it->first)); line.append(", ");
        line.append(std::to_string((float) it->first / (float) count_word)); line.append("\n");
        out.write(line.c_str(), line.length());
    }

    input.close();
    out.close();
    return 0;
}
