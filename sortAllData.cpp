//
// Created by dilu on 23-11-23.
//

#include <iostream>
#include "common/common.h"

int main() {
     {
        auto startTime = std::chrono::high_resolution_clock::now();

        sortBlocks();
        auto endTime = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> duration = endTime - startTime;
        double seconds = duration.count();

        std::cout << "临时文件排序执行时间: " << seconds << " 秒" << std::endl;
    }

    {
        auto startTime = std::chrono::high_resolution_clock::now();

        getAllTempDataSortByThreads();
        auto endTime = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> duration = endTime - startTime;
        double seconds = duration.count();

        std::cout << "总文件排序执行时间: " << seconds << " 秒" << std::endl;
    }


    return 0;
}
