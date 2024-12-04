#pragma once

#include <iostream>

class Error{
public:
    static void PrintListOfCommands(){
        std::cout << "The following commands are available to you:" << std::endl;
        std::cout << "dump <filename> - save the universe to a file" << std::endl;
        std::cout << "tick <n=1> - calculate n (default 1) iterations and print the result" << std::endl;
        std::cout << "t <n=1> - same as \"tick <n=1>\"" << std::endl;
        std::cout << "exit - finish the game" << std::endl;
        std::cout << "help - print command help" << std::endl;
        std::cout << "draw - show window with your plane" << std::endl;
    }
    class Command{
    public:
        static void print_error_command(const std::string & command){
            std::cout << "The entered command \"" << command <<"\" does not match the pattern" << std::endl;
            PrintListOfCommands();
        }
        static void print_error_argument(const std::string & arg){
            std::cout << "The entered command mustn't have some arguments, but command has argument:" << arg << std::endl;
            PrintListOfCommands();
        }
    };
    class File{
    public:
        static void error_file(const std::string & filename) {
            std::cout << "Unable to open file with filename \"" << filename << "\", default script will be run" << std::endl;
        }
        static void error_format(const std::string & filename){
            std::cout << "Incorrect format of file with filename \"" << filename << "\", default script will be run" << std::endl;
        }
        static void error_data(){
            std::cout << "Data error in input file" << std::endl;
        }
    };
    class Parameters{
    public:
        static void print_error(char* arg){
            std::cout << "Argument \"" << arg << "\" not found. Default script will be run" << std::endl;
            std::cout << "List of arguments command line:" << std::endl;
            std::cout << R"("-f <filename>" or "--input=filename" - input file)" << std::endl;
            std::cout << R"("-i <n>" or "--iteration=n" - count of iteration)" << std::endl;
            std::cout << R"("-o <filename>" or "--output=filename" - output file)" << std::endl;
        }
    };
};
