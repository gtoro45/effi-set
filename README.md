# effi-set
effi-set: A cache-conscious, accurate uniqueness-tracking data structure. Written in C++

effi-set works similarly to `std::unordered_set` in that it is a container of unique objects. However, `std::unordered_set` stores object data, which is a problem in situations where memory usage is limited. The `effi::effi_set` data structure has two simple aims:
* to reduce memory overhead when the programmer only desires to track uniqueness of large objects, such as `std::string`.
* to reduce false positives rates that lighter-weight containers (such as bloom filters) deal with

`effi::effi_set` uses  Robin Hood (RH) hashing, which is a fantastically cache-efficient addressing algorithm. Currently, the implemenation uses parallel `width_type` and `uint8_t` arrays for hash storage and algoritm metadata, respectively. In the future, I want to consider a packed struct approach to maintain everything in one contiguous cache line. 
* [martinus's RH map and set implementation](https://github.com/martinus/robin-hood-hashing)
* [GeeksforGeeks RH hashing explanation](https://www.geeksforgeeks.org/dsa/robin-hood-hashing/)

## Usage

## Bloom Filters and The Birthday Paradox

## effi-set and The Birthday Paradox
