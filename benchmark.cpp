#include "benchmark.h"

// custom hash function for std::vector<int>
uint64_t vector_hasher(const std::vector<int>& vec) {
    uint64_t seed = 0;
    for (int i : vec) {
        // A common way to combine hashes (similar to boost::hash_combine)
        seed ^= std::hash<int>{}(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
}

// verify the GCC Small String Optimization Limit to correctly estimate unordered_set capacity
void verify_sso(const std::string& s) {
    const void* string_object_addr = &s;
    const void* internal_data_addr = s.data();
    size_t string_object_size = sizeof(s);

    // Check if the data pointer points to a location within the object's own 32/24 bytes
    bool is_sso = (internal_data_addr >= string_object_addr && 
                   internal_data_addr < (char*)string_object_addr + string_object_size);

    std::cout << "String: \"" << s << "\" (Length: " << s.length() << ")\n";
    std::cout << "  Object Addr: " << string_object_addr << "\n";
    std::cout << "  Data Addr:   " << internal_data_addr << "\n";
    
    if (is_sso) {
        std::cout << "  RESULT: [ SSO ] (len=" << s.length() << " Stored inside the object)\n";
    } else {
        std::cout << "  RESULT: [ HEAP ] (len=" << s.length() << " Stored on the heap)\n";
    }
    std::cout << "------------------------------------------\n";
}

int main() {
    printf("beginning line reading... ");
    std::vector<std::string> urls = read_lines("data/URL-input-1M-2025.txt");
    printf("done\n");

    /******* Baseline Benchmark (std::string) *******/
    std::unordered_set<std::string> uset;
    for(std::string s : urls) { uset.insert(s); }
    int expected_unique = uset.size();

    // goal --> estimate unordered_set capacity
    // account for the number of buckets
    double base_footprint = (uset.bucket_count() * sizeof(void*));   
    // then, account for heap-allocated nodes + data                                   
    for(std::string s : uset) { 
        // avoid small string optimization (SSO) double counting
        // results from verify_sso show cutoff is >15 on gcc
        if(s.size() > 15)
           base_footprint += (sizeof(std::string) + s.size() + sizeof(void*));
        // else 
        //     verify_sso(s);
    }    
    base_footprint = base_footprint / (1024 * 1024); // convert to MB
    
    printf("Estimated Baseline Footprint = %.2f MB\n", base_footprint);
    printf("Expected Unique Strings = %d\n\n", expected_unique);



    /******* Insertion Benchmarks *******/
    insertion_bench<std::string, 8>(urls, base_footprint, expected_unique);
    insertion_bench<std::string, 16>(urls, base_footprint, expected_unique);
    insertion_bench<std::string, 32>(urls, base_footprint, expected_unique);
    insertion_bench<std::string, 64>(urls, base_footprint, expected_unique);


    
    /******* Some Tests *******/
    // Custom Hash Function Test (std::vector not hashable by std::hash)
    // std::vector<int> vec;
    // for(int i = 0; i < 1000; i++) vec.push_back(i);
    // effi::effi_set<std::vector<int>, 64> vec_set64(&vector_hasher);
    // vec_set64.insert(vec);
    // printf("Memory Footprint = %f MB\n", vec_set64.memory_footprint(true));


    return 0;
}