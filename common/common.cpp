//
// Created by dilu on 23-11-22.
//

#include "common.h"

std::random_device rd{};
std::mt19937 gen(rd());

int fileNum = 0;
int width = 20;

int fileId = 0;
std::mutex fileIdMutex{};

std::vector<std::pair<std::string, long long>> filesNameAndLength{};

int inputBuffSize = 1 << 15;
int outputBuffSize = 1 << 25;

std::vector<tempFile> tempFiles{};

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

int
getFilesNumAndName(const std::string &folderPath, std::vector<std::pair<std::string, long long>> &_filesNameAndLength) {
    DIR *directory = opendir(folderPath.c_str());
    if (directory == nullptr) {
        std::cerr << "Failed to open directory." << std::endl;
        return -1;
    }

    struct dirent *entry;
    while ((entry = readdir(directory)) != nullptr) {
        std::string fileName = entry->d_name;
        if (endsWith(fileName, ".txt")) {
            std::cout << fileName << std::endl;
            auto filePath = folderPath + "/" + fileName;
            _filesNameAndLength.emplace_back(filePath, getFileLength(filePath));
        }
    }

    closedir(directory);

    std::cout << "Number of files in the folder: " << folderPath << ": " << _filesNameAndLength.size() << std::endl;

    return _filesNameAndLength.size();
}

long long getFileLength(const std::string &filePath) {
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

bool getBlockSort(int id, long long offset, long long count) {
    std::vector<long long> result(count);
    size_t pos = 0;
    while (count) {
        if (id >= filesNameAndLength.size()) {
            break;
        }
        auto fileName = filesNameAndLength[id].first;
        auto fileLength = filesNameAndLength[id].second;
        ++id;
        std::fstream in(fileName);
        if (in.is_open()) {
            in.seekg(offset, std::ios::beg);
            std::string line;
            while (std::getline(in, line) && count) {
                long long value;
                std::istringstream iss(line);
                if (iss >> value) {
                    result[pos++] = value;
                }
                --count;
            }
            if (count) {
                offset = 0;
            }
            in.close();
            std::cout << "Array read successfully." << std::endl;
        } else {
            std::cout << "Unable to open file for reading." << std::endl;
        }
    }

    if (count) {
        result.resize(result.size() - count);
    }

    std::sort(result.begin(), result.end());

    std::unique_lock<std::mutex> lock(fileIdMutex);
    std::string outFileName = "allSortedDataByThreads/level0/data" + std::to_string(fileId) + ".txt";
    ++fileId;
    lock.unlock();

    std::ofstream file(outFileName);
    if (file.is_open()) {
        for (long long decValue: result) {
            file << std::setw(width) << std::dec << decValue << '\n';
        }

        file.close();
        std::cout << "Data has been written to file." << std::endl;
    } else {
        std::cout << "Unable to open file for writing." << std::endl;
    }

    return false;
}

void sortBlocks() {
    std::string folderPath = "allSortedDataByThreads";
    mkdir(folderPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    folderPath += "/level0";
    mkdir(folderPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    getFilesNumAndName("data", filesNameAndLength);
    ThreadPool threadPool(4);

    int id = 0;
    long long offset = 0;
    bool flag = true;
    long long count = 1 << 20;
    while (flag) {
        threadPool.enqueue(getBlockSort, id, offset, count);
        auto sum = count;
        while (sum) {
            if (filesNameAndLength[id].second - offset < sum * 21) {
                sum -= (filesNameAndLength[id].second - offset) / 21;
                if (id + 1 >= filesNameAndLength.size()) {
                    flag = false;
                    break;
                }
                ++id;
                offset = 0;
            } else {
                offset = offset + sum * 21;
                break;
            }
        }
    }
}

void getTempFilesName(const std::string &folderPath) {
    DIR *directory = opendir(folderPath.c_str());
    if (directory == nullptr) {
        std::cerr << "Failed to open directory." << std::endl;
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(directory)) != nullptr) {
        std::string fileName = entry->d_name;
        if (endsWith(fileName, ".txt")) {
            std::cout << fileName << std::endl;
            auto filePath = folderPath + "/" + fileName;
            tempFiles.emplace_back(filePath);
        }
    }

    closedir(directory);

    std::cout << "Number of files in the folder: " << folderPath << ": " << tempFiles.size() << std::endl;
}

void getAllTempDataSort() {
    getTempFilesName("allSortedDataByThreads/level0");
    for (auto &tempFile: tempFiles) {
        tempFile.getInputBuff();
    }
    std::priority_queue<std::pair<long long, int>, std::vector<std::pair<long long, int>>, std::greater<std::pair<long long, int>>> priorityQueue;
    for (int i = 0; i < tempFiles.size(); ++i) {
        priorityQueue.emplace(tempFiles[i].getNextNum(), i);
    }

    std::ofstream file("allSortedData/allSortedData.txt");
    int outputBuffPos = 0;
    std::vector<long long> outputBuff(outputBuffSize);

    while (!priorityQueue.empty()) {
        if (outputBuffPos >= outputBuff.size() && file.is_open()) {
            for (long long decValue: outputBuff) {
                file << std::setw(width) << std::dec << decValue << '\n';
            }
            outputBuffPos = 0;
        }
        auto item = priorityQueue.top();
        priorityQueue.pop();
        outputBuff[outputBuffPos++] = item.first;
        if (!tempFiles[item.second].isEnd || tempFiles[item.second].usedBuffSize) {
            priorityQueue.emplace(tempFiles[item.second].getNextNum(), item.second);
        }
    }

    if (file.is_open()) {
        for (int i = 0; i < outputBuffPos; ++i) {
            file << std::setw(width) << std::dec << outputBuff[i] << '\n';
        }
        outputBuffPos = 0;
    }
}

void getTwoTempDataSort(std::string filePath1, std::string filePath2, int level) {
    std::unique_lock<std::mutex> lock(fileIdMutex);
    std::string outFileName =
            "allSortedDataByThreads/level" + std::to_string(level) + "/data" + std::to_string(fileId) + ".txt";
    ++fileId;
    lock.unlock();

    if (filePath2.empty()) {

        std::ifstream sourceFile(filePath1, std::ios::binary);
        std::ofstream destinationFile(outFileName, std::ios::binary);

        if (sourceFile && destinationFile) {
            destinationFile << sourceFile.rdbuf();
        }
        return;
    }

    tempFile file1(filePath1), file2(filePath2);
    file1.getInputBuff();
    file2.getInputBuff();

    std::priority_queue<std::pair<long long, int>, std::vector<std::pair<long long, int>>, std::greater<std::pair<long long, int>>> priorityQueue;
    priorityQueue.emplace(file1.getNextNum(), 1);
    priorityQueue.emplace(file2.getNextNum(), 2);

    std::ofstream file(outFileName);
    int outputBuffPos = 0;
    int outBuffSize = 1 << 23;
    std::vector<long long> outputBuff(outBuffSize);

    while (!priorityQueue.empty()) {
        if (outputBuffPos >= outputBuff.size() && file.is_open()) {
            for (long long decValue: outputBuff) {
                file << std::setw(width) << std::dec << decValue << '\n';
            }
            outputBuffPos = 0;
        }
        auto item = priorityQueue.top();
        priorityQueue.pop();
        outputBuff[outputBuffPos++] = item.first;
        if ((!file1.isEnd || file1.usedBuffSize) && (!file2.isEnd || file2.usedBuffSize)) {
            if (item.second == 1) {
                priorityQueue.emplace(file1.getNextNum(), 1);
            } else {
                priorityQueue.emplace(file2.getNextNum(), 2);
            }
        }
    }

    while (!file1.isEnd || file1.usedBuffSize) {
        if (outputBuffPos >= outputBuff.size() && file.is_open()) {
            for (long long decValue: outputBuff) {
                file << std::setw(width) << std::dec << decValue << '\n';
            }
            outputBuffPos = 0;
        }
        outputBuff[outputBuffPos++] = file1.getNextNum();
    }

    while (!file2.isEnd || file2.usedBuffSize) {
        if (outputBuffPos >= outputBuff.size() && file.is_open()) {
            for (long long decValue: outputBuff) {
                file << std::setw(width) << std::dec << decValue << '\n';
            }
            outputBuffPos = 0;
        }
        outputBuff[outputBuffPos++] = file2.getNextNum();
    }

    if (file.is_open()) {
        for (int i=0;i<outputBuffPos;++i) {
            file << std::setw(width) << std::dec << outputBuff[i] << '\n';
        }
        outputBuffPos = 0;
    }
}


void getAllTempDataSortByThreads() {

    std::vector<std::pair<std::string, long long>> filesName;
    std::string folderPath = "allSortedDataByThreads/level0";
    int level = 1;

    while (getFilesNumAndName(folderPath, filesName) > 1) {

        auto files = filesName;

        ThreadPool threadPool(4);

        std::unique_lock<std::mutex> lock(fileIdMutex);
        fileId = 0;
        lock.unlock();

        folderPath = "allSortedDataByThreads/level" + std::to_string(level);
        mkdir(folderPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        for (int i = 0; i < files.size(); i += 2) {
            if (i + 1 < files.size()) {
                threadPool.enqueue(getTwoTempDataSort, files[i].first, files[i + 1].first, level);
            } else {
                threadPool.enqueue(getTwoTempDataSort, files[i].first, "", level);
            }
        }
        ++level;

        filesName.resize(0);
    }
}
