//
// Created by dilu on 23-11-20.
//

#include <iostream>
#include <random>
#include <fstream>
#include <iomanip>
#include <climits>

std::random_device rd;
std::mt19937 gen(rd());

long long randomNum(long long a, long long b) {
    std::uniform_int_distribution<long long> dis(a, b);
    return dis(gen);
}

int fileNum = 0;
int width = 20;

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

int main() {
    long long all = 1 << 25, sum = all;
    long long minCount = 1 << 6, maxCount = std::max(all >> 6, minCount);

    while (sum >= minCount) {
        long long count = randomNum(minCount, maxCount);
        generateOneFile(count);
        sum -= count * (width+1);
    }
    return 0;
}
