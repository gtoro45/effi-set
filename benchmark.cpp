#include "effi_set.h"
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>

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

    // Baseline comparison
    std::unordered_set<std::string> uset;
    double footprint = sizeof(std::string);
    for(std::string s : urls) {
        uset.insert(s);
        footprint += s.size();
    }
    footprint = footprint / 1000000; // convert to MB
    int expected_unique = uset.size();
    printf("Estimated Baseline Footprint = %f MB\n", footprint);
    printf("Expected Unique Strings = %d\n\n", expected_unique);

    // Precision Tests
    effi::effi_set<std::string, 8> set8;
    for(std::string s : urls) set8.insert(s);
    printf("8-bit Memory Footprint = %f MB\n", set8.memory_footprint(true));
    int size = set8.size();
    double loss = 1 - ((double)size / (double)expected_unique);
    printf("Stored Unique Strings = %d MB (%.2f%% loss)\n\n ", size, loss * 100);

    effi::effi_set<std::string, 16> set16;
    for(std::string s : urls) set16.insert(s);
    printf("16-bit Memory Footprint = %f MB\n", set16.memory_footprint(true));
    size = set16.size();
    loss = 1 - ((double)size / (double)expected_unique);
    printf("Stored Unique Strings = %d MB (%.2f%% loss)\n\n ", size, loss * 100);
    
    effi::effi_set<std::string, 32> set32;
    for(std::string s : urls) set32.insert(s);
    printf("32-bit Memory Footprint = %f MB\n", set32.memory_footprint(true));
    size = set32.size();
    loss = 1 - ((double)size / (double)expected_unique);
    printf("Stored Unique Strings = %d MB (%.2f%% loss)\n\n ", size, loss * 100);
    
    effi::effi_set<std::string, 64> set64;
    for(std::string s : urls) set64.insert(s);
    printf("64-bit Memory Footprint = %f MB\n", set64.memory_footprint(true));
    size = set64.size();
    loss = 1 - ((double)size / (double)expected_unique);
    printf("Stored Unique Strings = %d MB (%.2f%% loss)\n\n ", size, loss * 100);

    // std::vector<int> vec;
    // for(int i = 0; i < 1000; i++) vec.push_back(i);
    // effi::effi_set<std::vector<int>, 64> vec_set64(&vector_hasher);
    // vec_set64.insert(vec);
    // printf("Memory Footprint = %f MB\n", vec_set64.memory_footprint(true));


    return 0;
}