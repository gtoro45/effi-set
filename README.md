# effi-set
effi-set: A cache-conscious, accurate uniqueness-tracking data structure. Written in C++

## Usage
effi-set works similarly to `std::unordered_set` in that it is a container of unique objects. However, `std::unordered_set` stores object data, which is a problem in situations where memory usage is limited. The `effi_set` data structure aims to reduce memory overhead when the programmer only desires to track uniqueness of large objects, such as `std::string`.

## Bloom Filters and The Birthday Paradox

## effi-set and The Birthday Paradox
