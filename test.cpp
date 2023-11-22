//
// Created by dilu on 23-11-22.
//

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <chrono>
#include <algorithm>
#include <sstream>
#include "common/common.h"

int main() {
    int numCount=1 << 20;
    std::vector<long long> numbers(numCount);
    for (auto &it: numbers) {
        it = randomNum(LLONG_MIN, LLONG_MAX);
    }

    {
        auto startTime = std::chrono::high_resolution_clock::now();

        std::sort(numbers.begin(), numbers.end());

        auto endTime = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> duration = endTime - startTime;
        double seconds = duration.count();

        std::cout << "排序代码执行时间: " << seconds << " 秒" << std::endl;
    }

    {
        auto startTime = std::chrono::high_resolution_clock::now();

        int hexWidth = 20;

        std::ofstream file("data.txt");
        if (file.is_open()) {
            for (int i = 0; i < numCount; ++i) {
                long long hexValue = numbers[i];
                file << std::setw(hexWidth) << std::dec << hexValue << '\n';
            }

            file.close();
            std::cout << "Data has been written to file." << std::endl;
        } else {
            std::cout << "Unable to open file for writing." << std::endl;
        }

        auto endTime = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> duration = endTime - startTime;
        double seconds = duration.count();

        std::cout << "存文件代码执行时间: " << seconds << " 秒" << std::endl;
    }

    {
        auto startTime = std::chrono::high_resolution_clock::now();

        std::vector<long long> result(numCount);

        std::ifstream file("data.txt");
        if (file.is_open()) {
            std::string line;
            int i=0;
            while (std::getline(file, line)) {
                long long value;
                std::istringstream iss(line);
                if (iss >> value) {
                    result[i++]=value;
                }
            }
            file.close();
            std::cout << "Array read successfully." << std::endl;
        } else {
            std::cout << "Unable to open file for reading." << std::endl;
        }

        auto endTime = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> duration = endTime - startTime;
        double seconds = duration.count();

        std::cout << "读文件代码执行时间: " << seconds << " 秒" << std::endl;

    }

    return 0;
}
