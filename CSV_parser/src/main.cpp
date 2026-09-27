#include "PrintTuple.h"
#include "CSVParser.h"

int main() {
    Settings settings;
    std::ifstream file("test.csv");
    CSVParser<std::string, int, float> parser(file, 1, settings);
    for (auto it = parser.begin(); it != parser.end(); ++it) {
        std::cout << *it;
        std::cout << std::endl;
    }
    return 0;
}
