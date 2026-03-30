// effi_set.h
#include <iostream>
#include <functional>
#include <string>

template <typename T, int precision> 
class effi_set {
    private:
        // std::conditional for compile-time type determination
        using width_type = 
            std::conditional_t<precision <= 8,   uint8_t,
            std::conditional_t<precision <= 16,  uint16_t,
            std::conditional_t<precision <= 32,  uint32_t, uint64_t>>>;
        
        width_type* array;                      // the internal set array
        int _size;                              // the set's size visible to the user
        int _capacity;                          // current set capacity
        width_type (*hash_logic)(const T&);     // function pointer to the hash function used by the set 
        

    public:
        /************  Internal Hash Function ************/
        static width_type hash_function(const T& object) {
            width_type _hash = std::hash<T>{}(object);
            return _hash;
        }
        /************  Constructor(s) ************/
        // Default Constructor
        effi_set() : array(nullptr), _size(0), _capacity(0), hash_logic(&hash_function) {}
        
        // Custom Hash Function Constructor
        effi_set(width_type (*custom_hash)(const T&)) : array(nullptr), _size(0), _capacity(0), hash_logic(custom_hash) {}

        // Destructor
        ~effi_set() { delete[] array; }

        /************  Class Methods ************/
        int insert(T object) {
            width_type object_hash = this->hash_logic(object);
            printf("Object Hash on insert call: %lu\n", object_hash);
            // TODO
            return 0;
        }

        int contains(T object) {
            // TODO
            return 0;
        }
        
        int remove(T object) {
            // TODO
            return 0;
        }

        int clear() {
            // TODO
            return 0;
        }

        int size() { return this->_size; }
        int capacity() { return this->_capacity; }
};