// benchmark.h
#include "effi_set.h"
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>

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


template <typename T, int precision>
void insertion_bench(std::vector<T>& data, double expected_memory_MB, int expected_size) {
    effi::effi_set<T, precision> eset;
    double loss;
    double comp_ratio;
    
    for(T t : data) eset.insert(t);
    double footprint_MB = eset.memory_footprint(true);
    printf("%d-bit Memory Footprint = %f MB\n", precision, footprint_MB);
    int size = eset.size();
    loss = 1 - ((double)size / (double)expected_size);
    comp_ratio = expected_memory_MB / footprint_MB;
    printf("Stored Unique Strings = %d (%.3f%% loss, %.2fx compression)\n\n", size, loss * 100, comp_ratio);
}

