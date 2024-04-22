#include <iostream>
#include <vector>
#include <optional>
#include <array>

constexpr size_t sizeX{ 7 };
constexpr size_t sizeY{ 3 };

template<typename T>
struct Data {
    T value;
    uint8_t count;
};

template<typename T>
using Array2D = std::array<std::array<T, sizeX>, sizeY>;

template<typename T>
using CompressedData = std::vector<std::vector<Data<T>>>;

template<typename T>
std::optional<CompressedData<T>> compressData(const Array2D<T>& input) {
    CompressedData<T> compressed;
    for (const auto& row : input) {
        std::vector<Data<T>> compressed_row;
        T prev_value = row[0];
        uint8_t count = 1;
        for (size_t i = 1; i < sizeX; ++i) {
            if (row[i] == prev_value) {
                count++;
            }
            else {
                compressed_row.push_back(Data<T>{prev_value, count});
                prev_value = row[i];
                count = 1;
            }
        }
        compressed_row.push_back(Data<T>{prev_value, count});
        compressed.push_back(compressed_row);
    }

    size_t compressed_size = 0;
    for (const auto& row : compressed) {
        compressed_size += row.size();
    }
    if (compressed_size >= sizeX * sizeY) {
        return std::nullopt;
    }

    return compressed;
}

void printCompressedData(const CompressedData<int>& data) {
    for (const auto& row : data) {
        std::cout << "{";
        for (const auto& item : row) {
            std::cout << "{" << (int)item.value << "," << (int)item.count << "}";
            if (&item != &row.back()) {
                std::cout << ",";
            }
        }
        std::cout << "}";
        if (&row != &data.back()) {
            std::cout << ",\n";
        }
    }
}

#ifndef UNIT_TESTING
int main() {
    Array2D<int> testArray{ {
        {0, 0, 0, 1, 1, 2, 3},
        {0, 0, 4, 4, 4, 2, 2},
        {2, 2, 2, 2, 2, 1, 2}
    } };

    auto compressed = compressData(testArray);
    if (compressed) {
        printCompressedData(compressed.value());
    }
    else {
        std::cout << "Data could not be compressed or compressed data is not smaller." << std::endl;
    }

    return 0;
}
#endif
