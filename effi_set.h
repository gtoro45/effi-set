// effi_set.h
#include <iostream>
#include <functional>
#include <string>

namespace effi {
    #define INITIAL_SET_CAP 10
    #define MASK_56(a) (uint64_t)(a) & 0xFFFFFFFFFFFFFF;

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

            // the Probe Sequence Length (PSL) for each element (required for the robinhood algorithm)
            // this is parallel to the internal array 
            uint8_t* PSLs;
            
            // the set's size visible to the user
            int _size;                              
            
            // current set capacity
            int _capacity;         

            // function pointer to the hash function used by the set
            width_type (*hash_logic)(const T&);      

            // grow the internal array when at/near capacity 
            void grow() {                           
                if(array != nullptr){
                    /********************* RH Resizing *********************/
                    // (0) double capacity, keep track of previous
                    int old_capacity = _capacity;
                    _capacity *= 2; // this can be optimized

                    // (1) clear and resize PSLs (data is garbage atp)
                    delete[] PSLs;
                    PSLs = nullptr;
                    PSLs = new uint8_t[_capacity]();
                    
                    // (2) copy elements into temp array, then clear and resize main array
                    width_type* temp_arr = new width_type[_capacity](); // can be optimized to old_capacity, will do so later
                    for(int i = 0; i < old_capacity; i++) temp_arr[i] = std::move(this->array[i]);
                    delete[] array;
                    array = nullptr;

                    // (3) re-insert elements from temp_arr (clear _size before insertion increments it)
                    _size = 0;
                    array = new width_type[_capacity]();
                    for(int i = 0; i < old_capacity; i++) {
                        if(temp_arr[i] != 0) {
                            insert(temp_arr[i]);
                        }
                    }

                    // (4) clear the temp array
                    delete[] temp_arr;
                    temp_arr = nullptr;
                    /*******************************************************/
                }
                else {
                    array = new width_type[INITIAL_SET_CAP]();
                    PSLs = new uint8_t[INITIAL_SET_CAP]();
                    _capacity = INITIAL_SET_CAP;
                    _size = 0;
                }
            }
            

        public:
            /************  Internal Hash Function ************/
            static width_type hash_function(const T& object) {
                width_type _hash = std::hash<T>{}(object);
                return _hash;
            }

            /************  Constructor(s) ************/
            // Default Constructor
            effi_set() : array(new width_type[INITIAL_SET_CAP]()), 
                        PSLs(new uint8_t[INITIAL_SET_CAP]()),
                        _size(0), 
                        _capacity(INITIAL_SET_CAP), 
                        hash_logic(&hash_function) {}
            
            // Custom Hash Function Constructor
            effi_set(width_type (*custom_hash)(const T&)) : array(new width_type[INITIAL_SET_CAP]()), 
                                                            PSLs(new uint8_t[INITIAL_SET_CAP]()),
                                                            _size(0), 
                                                            _capacity(INITIAL_SET_CAP), 
                                                            hash_logic(custom_hash) {}

            // Destructor
            ~effi_set() { 
                delete[] array;
                delete[] PSLs;
            }

            /************  Class Methods ************/
            int insert(T object) {
                width_type object_hash = this->hash_logic(object);
                insert(object_hash);
                return 0;
            }

            int insert(width_type object_hash) {
                // grow internal array at 75% capacity
                if(this->_size >= this->_capacity * 0.75) {
                    this->grow();
                }
                
                width_type idx = object_hash % _capacity;
                uint8_t psl = 0;
                while(array[idx] != 0) {
                    // don't insert on existence (whole point)
                    if(object_hash == array[idx]) return 1;

                    // Robin Hood:  steal from the rich (low PSL)
                    //              give to the poor (high PSL)
                    //              std::swap: both seats the winner (poor) 
                    //              and evicts the loser (rich)
                    //              to find them a new home further down
                    if(psl > PSLs[idx]) {
                        std::swap(object_hash, array[idx]); // object_hash is now inserted, now probing for the element previously at array[idx]
                        std::swap(psl, PSLs[idx]);          // psl is now inserted, now probing for the element previously at PSLs[idx]
                    }
                    
                    // standard linear probing
                    idx = (idx + 1) % _capacity;
                    psl++;
                }

                array[idx] = object_hash;
                PSLs[idx] = psl;
                this->_size++;
                
                return 0;
            }

            bool contains(T object) {
                width_type object_hash = this->hash_logic(object);
                width_type idx = object_hash % _capacity;
                uint8_t psl = 0;

                while(array[idx] != 0) {
                    // return on successful retrieval
                    if(object_hash == array[idx]) return true;

                    // Robin Hood:  if we have searched farther than the element
                    //              in current slot has traveled, our hash cannot
                    //              exist further down the line
                    if(psl > PSLs[idx]) return false;

                    // standard linear probing
                    idx = (idx + 1) % _capacity;
                    psl++;
                }

                // only reachable if loop condition isn't met
                // i.e., no such hash exists
                return false;
            }
            
            int remove(T object) {
                // TODO
                return 0;
            }

            void clear() {
                // clear members, in order
                delete[] this->array;
                delete[] this->PSLs;
                this->array = nullptr;
                this->PSLs = nullptr;
                this->_size = 0;
                this->_capacity = 0;
            }

            int size() { return this->_size; }
            
            int capacity() { return this->_capacity; }
            
            double memory_footprint(bool MB) { 
                // TODO: check if T is iterable and add sizeof its elements
                double bytes = (sizeof(width_type) * this->_capacity) + 
                                (sizeof(uint8_t) * this->_capacity) +
                                sizeof(this->_size) +
                                sizeof(this->_capacity) +
                                sizeof(width_type*); 
                return (MB) ? bytes / (1024 * 1024) : bytes;
            }
    };


    // TODO: protoyping so I don't forget
    // template <typename T, int precision> 
    // class effi_set_packed_64 {
    //     private:
    //         // single element of the internal array (4-byte aligned)
    //         struct element {
    //                 uint64_t hash : 56;     // 56-bit hash
    //                 uint64_t pcl : 8;       // 8-bit pcl for Robin Hood
    //         };

    //     public:
    //         // TODO
    // };
}
