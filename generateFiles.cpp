//
// Created by dilu on 23-11-20.
//

#include <iostream>
#include "common/common.h"

int main() {
    mkdir("data", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    long long all = 1 << 25, sum = all;
    long long minCount = 1 << 6, maxCount = std::max(all >> 6, minCount);

    while (sum >= minCount * (width + 1)) {
        long long count = randomNum(minCount, maxCount);
        generateOneFile(count);
        sum -= count * (width + 1);
    }
    return 0;
}
