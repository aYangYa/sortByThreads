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

extern std::random_device rd;
extern std::mt19937 gen;

extern int fileNum;
extern int width;

long long randomNum(long long a, long long b);

void generateOneFile(long long count);

#endif //SORTBYTHREADS_COMMON_H
