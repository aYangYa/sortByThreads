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

bool endsWith(const std::string &str, const std::string &suffix) {
    return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

int getFilesNumAndName(const std::string &folderPath, std::vector<std::string> &filesName) {
    DIR *directory = opendir(folderPath.c_str());
    if (directory == nullptr) {
        std::cerr << "Failed to open directory." << std::endl;
        return -1;
    }

    int fileCount = 0;
    struct dirent *entry;
    while ((entry = readdir(directory)) != nullptr) {
        if (entry->d_type == DT_REG) {  // 只计数常规文件
            fileCount++;
        }
        std::string fileName = entry->d_name;
        if (endsWith(fileName, ".txt")) {
            std::cout << fileName << std::endl;
            filesName.emplace_back(folderPath+"/"+fileName);
        }
    }

    closedir(directory);

    std::cout << "Number of files in the folder: " << fileCount << std::endl;

    return fileCount;
}

int getFileLength(const std::string &filePath) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Failed to open the file." << std::endl;
        return -1;
    }

    std::streampos fileSize = file.tellg();
    file.close();

    std::cout << "File size: " << fileSize << " bytes" << std::endl;

    return fileSize;
}