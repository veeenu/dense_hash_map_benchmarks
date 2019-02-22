# Google Dense Hash Map Benchmarks

This is a simple benchmark to compare the performance of [Google's Sparsehash C++11](https://github.com/sparsehash/sparsehash-c11)'s
implementation of `dense_hash_map` against C++11 `std::unordered_map` for `std::string` keys. This is borne out of the need to have
a more performant key access over a finalized key set with no further insertions and structurally similar keys (uppercase financial
tickers) where `std::unordered_map` was suspected to be a bottleneck. On recent MSVC with optimizations enabled, the results are
interesting:

```
Reading tickers list... 4.47ms
Inserting:
  unordered_map:  3.77ms
  dense_hash_map: 6.97ms
Random access:
  unordered_map:  8.39s
  dense_hash_map: 1.39s
Single key:
  unordered_map:  3.69s
  dense_hash_map: 285.00ns
```

`unordered_map` has a much faster insertion, but is clearly ill suited for massive lookups in polynomial, long running algorithms.

The access tests are conceived as such:

- The first test iterates over a (separate) vector of all the keys in the map and accesses the key, obliterating the cache.
- The second test accesses a single key over and over.

In both cases the speedup is significant enough to warrant switching over for this particular use case.

# To-Do List

- Introduce a "shuffled vector lookup" test: lookup the keys in the order established by a randomly (but predictably) shuffled vector to eliminate potential caching effects due to contiguity/closeness of keys with the default ordering -- I can only speculate this may or may not have an effect as I'm not aware of the inner workings of the two map implementations in this sense, that's why I should test it.
- Improve the benchmark's statistical significance by running each test multiple times and plotting the sample distribution.