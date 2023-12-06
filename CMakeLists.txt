cmake_minimum_required(VERSION 3.23)
project (sortByThreads)

set(CMAKE_CXX_STANDARD 11)

add_executable(readFromFile readFromFile.cpp)
add_executable(generateFiles generateFiles.cpp common/common.cpp)
target_link_libraries(generateFiles pthread)
add_executable(test test.cpp common/common.cpp)
add_executable(sortAllData sortAllData.cpp common/common.cpp)
target_link_libraries(sortAllData pthread)