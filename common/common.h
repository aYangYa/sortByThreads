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

extern std::random_device rd;
extern std::mt19937 gen;

extern int fileNum;
extern int width;

long long randomNum(long long a, long long b);

void generateOneFile(long long count);

bool endsWith(const std::string &str, const std::string &suffix);

int getFilesNumAndName(const std::string &folderPath, std::vector<std::string> &filesName);

int getFileLength(const std::string &filePath);

#endif //SORTBYTHREADS_COMMON_H
