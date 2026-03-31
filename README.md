# effi-set
effi-set: A cache-conscious, accurate uniqueness-tracking data structure. Written in C++

effi-set works similarly to `std::unordered_set` in that it is a container of unique objects. However, `std::unordered_set` stores object data, which is a problem in situations where memory usage is limited. The `effi::effi_set` data structure has two simple aims:
* to reduce memory overhead when the programmer only desires to track uniqueness of large objects, such as `std::string`.
* to reduce false positives rates that lighter-weight containers (such as bloom filters) deal with

`effi::effi_set` uses  Robin Hood (RH) hashing, which is a fantastically cache-efficient addressing algorithm. Currently, the implemenation uses parallel `width_type` and `uint8_t` arrays for hash storage and algoritm metadata, respectively. In the future, I want to consider a packed struct approach to further exploit spatial locality. 

* [martinus's RH map and set implementation](https://github.com/martinus/robin-hood-hashing)
* [GeeksforGeeks RH hashing explanation](https://www.geeksforgeeks.org/dsa/robin-hood-hashing/)

Inspiration for this project came from my CSCE 463 class at Texas A&M, where in one of the homeworks we were asked to make sure our web crawler's memory overhead did not exceed 300MB. I was using `std::unodered_set<string>`, which was bloating up my program (1M hosts being checked for uniqueness). I had a "lightbulb moment" and traded off some extra cycles to compute a 64-bit hash of each host string and store that in the `unordered_set` container. This resulted in 40% lower memory usage. In the same project (for the same problem) I considered using `robin_hood::unordered_set` for program speedup, but no external libraries were allowed unfortunately. So, here I am combining both ideas.

## Usage

## Bloom Filters and The Birthday Paradox

## effi-set and The Birthday Paradox
