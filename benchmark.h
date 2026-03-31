// benchmark.h
#include "effi_set.h"
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <chrono>

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
void insertion_bench(std::vector<T>& data, double base_insert_ms, double expected_memory_MB, int expected_size) {
    // benchmark members
    effi::effi_set<T, precision> eset;
    double insert_duration_ms;
    double nanosec_per_insert;
    double loss;
    double comp_ratio;
    
    // time insertion of elements
    auto start = std::chrono::high_resolution_clock::now();
    for(T t : data) eset.insert(t);
    auto end = std::chrono::high_resolution_clock::now();
    insert_duration_ms = std::chrono::duration<double, std::milli>(end - start).count();
    nanosec_per_insert = (insert_duration_ms * 1e6) / data.size();
    printf("Total insert time = %.2f (%.2fx speedup) \n", insert_duration_ms, base_insert_ms / insert_duration_ms);
    printf("Time per insert (ns) = %.2f (%.2fx speedup) \n", nanosec_per_insert, base_insert_ms / insert_duration_ms);
    
    // calculate memory footprint 
    double footprint_MB = eset.memory_footprint(true);
    printf("%d-bit Memory Footprint = %f MB\n", precision, footprint_MB);
    
    // calculate loss rate (hashing collisions) and compression ratio
    int size = eset.size();
    loss = 1 - ((double)size / (double)expected_size);
    comp_ratio = expected_memory_MB / footprint_MB;
    printf("Stored Unique Strings = %d (%.3f%% loss, %.2fx compression)\n\n", size, loss * 100, comp_ratio);
}



template <typename T, int precision>
void access_bench(std::vector<T>& data, double base_access_ms) {
    // benchmark members
    effi::effi_set<T, precision> eset; for(const T& t : data) eset.insert(t);  // teehee
    
}