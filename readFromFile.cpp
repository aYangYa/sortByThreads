#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

std::vector<long long> readArray(const std::string& filename) {
    std::vector<long long> result;

    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            long long value;
            std::istringstream iss(line);
            if (iss >> value) {
                result.push_back(value);
            }
        }
        file.close();
        std::cout << "Array read successfully." << std::endl;
    } else {
        std::cout << "Unable to open file for reading." << std::endl;
    }

    return result;
}

int main() {
    std::string filename = "data.txt";
    std::vector<long long> readValues = readArray(filename);

    for (const auto& value : readValues) {
        std::cout << "Value: " << value << std::endl;
    }

    return 0;
}