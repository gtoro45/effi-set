#include "effi_set.h"
#include <vector>

uint64_t vector_hasher(const std::vector<int>& vec) {
    uint64_t seed = 0;
    for (int i : vec) {
        // A common way to combine hashes (similar to boost::hash_combine)
        seed ^= std::hash<int>{}(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
}

int main() {
    effi::effi_set<std::string, 8> set8;
    set8.insert("hello world");
    set8.insert("h3llo world");
    set8.insert("hello wor1d");
    printf("\n");

    effi::effi_set<std::string, 16> set16;
    set16.insert("hello world");
    set16.insert("h3llo world");
    set16.insert("hello wor1d");
    printf("\n");
    
    effi::effi_set<std::string, 32> set32;
    set32.insert("hello world");
    set32.insert("h3llo world");
    set32.insert("hello wor1d");
    printf("\n");
    
    effi::effi_set<std::string, 64> set64;
    set64.insert("hello world");
    set64.insert("h3llo world");
    set64.insert("hello wor1d");
    printf("\n");

    std::vector<int> vec;
    for(int i = 0; i < 1000; i++) vec.push_back(i);
    effi::effi_set<std::vector<int>, 64> vec_set64(&vector_hasher);
    vec_set64.insert(vec);


    return 0;
}