//
// Created by dilu on 23-11-23.
//

#include <iostream>
#include <vector>
#include "common/common.h"

int main() {
    std::vector<std::string> filesName;
    getFilesNumAndName("data", filesName);
    for (auto &name: filesName) {
        getFileLength(name);
    }
    return 0;
}
