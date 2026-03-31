// effi_set.h
#include <iostream>
#include <functional>
#include <string>

namespace effi {
    #define INITIAL_SET_CAP 10

    template <typename T, int precision> 
    class effi_set {
        private:
            // std::conditional for compile-time type determination
            using width_type = 
                std::conditional_t<precision <= 8,   uint8_t,
                std::conditional_t<precision <= 16,  uint16_t,
                std::conditional_t<precision <= 32,  uint32_t, uint64_t>>>;
            
            // the internal set array
            width_type* array;

            // the metadata array for each element (required by the robinhood algorithm)
            // this is parallel to the internal array 
            uint8_t* dibs;
            
            // the set's size visible to the user
            int _size;                              
            
            // current set capacity
            int _capacity;         

            // function pointer to the hash function used by the set
            width_type (*hash_logic)(const T&);      

            // grow the internal array when at/near capacity 
            void grow() {                           
                
            }
            

        public:
            /************  Internal Hash Function ************/
            static width_type hash_function(const T& object) {
                width_type _hash = std::hash<T>{}(object);
                return _hash;
            }

            /************  Constructor(s) ************/
            // Default Constructor
            effi_set() : array(new width_type[INITIAL_SET_CAP]), 
                        dibs(new uint8_t[INITIAL_SET_CAP])
                        _size(0), 
                        _capacity(INITIAL_SET_CAP), 
                        hash_logic(&hash_function) {}
            
            // Custom Hash Function Constructor
            effi_set(width_type (*custom_hash)(const T&)) : array(new width_type[INITIAL_SET_CAP]), 
                                                            dibs(new uint8_t[INITIAL_SET_CAP])
                                                            _size(0), 
                                                            _capacity(INITIAL_SET_CAP), 
                                                            hash_logic(custom_hash) {}

            // Destructor
            ~effi_set() { 
                delete[] array;
                delete[] dibs;
            }

            /************  Class Methods ************/
            int insert(T object) {
                // grow internal array at 75% capacity
                if(this->_size >= this->_capacity * 0.75) {
                    this->grow();
                }

                width_type object_hash = this->hash_logic(object);
                printf("Object Hash on insert call: %lu\n", object_hash);
                // TODO
                return 0;
            }

            bool contains(T object) {
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


    // TODO: protoyping so I don't forget
    // template <typename T, int precision> 
    // class effi_set_packed {
    //     private:
    //         // std::conditional for compile-time type determination
    //         using width_type = 
    //             std::conditional_t<precision <= 8,   uint8_t,
    //             std::conditional_t<precision <= 16,  uint16_t,
    //             std::conditional_t<precision <= 32,  uint32_t, uint64_t>>>;
    //             #pragma pack(push, 1)
    //             struct element {
    //             // width_type hash : precision - 8,
    //             // uint8_t pcl
    //             }
    //             #pragma pack(pop)
    // };
}
