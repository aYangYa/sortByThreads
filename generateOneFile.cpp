#include <iostream>
#include <fstream>
#include <iomanip>

int main() {
    long long numbers[] = {-10, -25500000000, -1000, 42};
    int numCount = sizeof(numbers) / sizeof(numbers[0]);
    int hexWidth = 20;

    std::ofstream file("data.txt");
    if (file.is_open()) {
        for (int i = 0; i < numCount; ++i) {
            long long hexValue = numbers[i];
            file << std::setw(hexWidth) << std::dec << hexValue << '\n';
        }

        file.close();
        std::cout << "Data has been written to file." << std::endl;
    } else {
        std::cout << "Unable to open file for writing." << std::endl;
    }

    return 0;
}