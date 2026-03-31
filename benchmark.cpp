#include "effi_set.h"
#include <fstream>
#include <string>
#include <vector>

uint64_t vector_hasher(const std::vector<int>& vec) {
    uint64_t seed = 0;
    for (int i : vec) {
        // A common way to combine hashes (similar to boost::hash_combine)
        seed ^= std::hash<int>{}(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
}

std::vector<std::string> read_lines(const std::string& filename) {
    std::vector<std::string> lines;
    std::ifstream file(filename);

    if (!file.is_open())
        return lines;

    std::string line;
    while (std::getline(file, line))
        lines.push_back(line);

    return lines;
}

int main() {
    printf("beginning line reading... ");
    std::vector<std::string> urls = read_lines("data/URL-input-1M-2025.txt");
    printf("done\n");

    effi::effi_set<std::string, 8> set8;
    for(std::string s : urls) set8.insert(s);
    printf("Memory Footprint = %dB\n", set8.memory_footprint());

    effi::effi_set<std::string, 16> set16;
    for(std::string s : urls) set16.insert(s);
    printf("Memory Footprint = %dB\n", set16.memory_footprint());
    
    effi::effi_set<std::string, 32> set32;
    for(std::string s : urls) set32.insert(s);
    printf("Memory Footprint = %dB\n", set32.memory_footprint());
    
    effi::effi_set<std::string, 64> set64;
    for(std::string s : urls) set64.insert(s);
    printf("Memory Footprint = %dB\n", set64.memory_footprint());

    std::vector<int> vec;
    for(int i = 0; i < 1000; i++) vec.push_back(i);
    effi::effi_set<std::vector<int>, 64> vec_set64(&vector_hasher);
    vec_set64.insert(vec);


    return 0;
}