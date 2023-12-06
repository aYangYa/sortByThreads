//
// Created by dilu on 23-11-22.
//

#ifndef SORTBYTHREADS_COMMON_H
#define SORTBYTHREADS_COMMON_H

//
// Created by dilu on 23-11-20.
//

#include <iostream>
#include <random>
#include <fstream>
#include <iomanip>
#include <climits>
#include <dirent.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <mutex>
#include <condition_variable>
#include <utility>
#include <queue>
#include <sys/stat.h>
#include "ThreadPool.h"


extern std::random_device rd;
extern std::mt19937 gen;

extern int fileNum;
extern int width;

extern std::vector<std::pair<std::string, long long>> filesNameAndLength;

extern int fileId;
extern std::mutex fileIdMutex;

extern int inputBuffSize;
extern int outputBuffSize;

class tempFile {
public:
    std::fstream in;
    std::vector<long long> inputBuff;
    int pos;
    int usedBuffSize;
    bool isEnd;

public:
    explicit tempFile(const std::string &_filePath) : in(_filePath), inputBuff(inputBuffSize), pos(0), usedBuffSize(0),
                                                      isEnd(
                                                              false) {
        if (!in.is_open()) {
            std::cerr << "can't open file: " << _filePath << std::endl;
        }
    }

    void getInputBuff() {
        std::string line;
        while (std::getline(in, line)) {
            long long value;
            std::istringstream iss(line);
            if (iss >> value) {
                inputBuff[usedBuffSize++] = value;
                if (usedBuffSize >= inputBuff.size()) {
                    return;
                }
            }
        }
        isEnd = true;
    }

    long long getNextNum() {
        --usedBuffSize;
        auto num = inputBuff[pos++];
        if (usedBuffSize == 0) {
            pos = 0;
            getInputBuff();
        }
        return num;
    }


};

extern std::vector<tempFile> tempFiles;

long long randomNum(long long a, long long b);

void generateOneFile(long long count);

bool endsWith(const std::string &str, const std::string &suffix);

int
getFilesNumAndName(const std::string &folderPath, std::vector<std::pair<std::string, long long>> &_filesNameAndLength);

long long int getFileLength(const std::string &filePath);

bool getBlockSort(int id, long long offset, long long count);

void getTempFilesName(const std::string &folderPath);

void getAllTempDataSort();

void sortBlocks();

void getTwoTempDataSort(std::string filePath1, std::string filePath2, int level);

void getAllTempDataSortByThreads();

#endif //SORTBYTHREADS_COMMON_H
