cmake_minimum_required(VERSION 3.23)
project (sortByThreads)

set(CMAKE_CXX_STANDARD 11)

add_executable(generateOneFile generateOneFile.cpp)
add_executable(readFromFile readFromFile.cpp)
add_executable(generateFiles generateFiles.cpp)
add_executable(test test.cpp common/common.cpp)