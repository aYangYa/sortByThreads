//
// Created by dilu on 23-11-22.
//

#include "common.h"

std::random_device rd{};
std::mt19937 gen(rd());

int fileNum = 0;
int width = 20;

long long randomNum(long long a, long long b) {
    std::uniform_int_distribution<long long> dis(a, b);
    return dis(gen);
}

void generateOneFile(long long count) {
    std::ofstream file("data/data" + std::to_string(fileNum++) + ".txt");
    if (file.is_open()) {
        for (long long i = 0; i < count; ++i) {
            long long hexValue = randomNum(LLONG_MIN, LLONG_MAX);
            file << std::setw(width) << std::dec << hexValue << '\n';
        }

        file.close();
        std::cout << "Data has been written to file." << std::endl;
    } else {
        std::cout << "Unable to open file for writing." << std::endl;
    }
}