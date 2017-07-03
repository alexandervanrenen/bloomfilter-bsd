#pragma once

#include <dtl/dtl.hpp>
#include <dtl/hash.hpp>
#include <dtl/mem.hpp>

#include <dtl/bloomfilter.hpp>
#include <dtl/bloomfilter2.hpp>

// use 'bloomfilter2' for k > 1 (used for benchmarking purposes only)
// #define USE_BF2

namespace dtl {

// TODO remove namespace pollution
using key_t = $u32;
using word_t = $u32;

template<typename T>
//using hash_fn_0 = dtl::hash::knuth<T>;
// -- alternative hash functions
//using hash_fn_0 = dtl::hash::murmur_32<T>;
using hash_fn_0 = dtl::hash::fnv_32<T>;
//using hash_fn_0 = dtl::hash::identity<T>;

template<typename T>
//using hash_fn_1 = dtl::hash::knuth_alt<T>;
// -- alternative hash functions
//using hash_fn_1 = dtl::hash::murmur_32_alt<T>;
using hash_fn_1 = dtl::hash::fnv_32_alt<T>;
//using hash_fn_1 = dtl::hash::identity<T>;


// The supported Bloom filter implementations. (Note: Sectorization is not supported via the runtime API.)
using bf1_k1_t = dtl::bloomfilter<key_t, hash_fn_0, word_t, dtl::mem::numa_allocator<word_t>, 1, false>;
#ifndef USE_BF2
using bf1_k2_t = dtl::bloomfilter<key_t, hash_fn_0, word_t, dtl::mem::numa_allocator<word_t>, 2, false>;
using bf1_k3_t = dtl::bloomfilter<key_t, hash_fn_0, word_t, dtl::mem::numa_allocator<word_t>, 3, false>;
using bf1_k4_t = dtl::bloomfilter<key_t, hash_fn_0, word_t, dtl::mem::numa_allocator<word_t>, 4, false>;
using bf1_k5_t = dtl::bloomfilter<key_t, hash_fn_0, word_t, dtl::mem::numa_allocator<word_t>, 5, false>;
using bf1_k6_t = dtl::bloomfilter<key_t, hash_fn_0, word_t, dtl::mem::numa_allocator<word_t>, 6, false>;
#else
using bf1_k2_t = dtl::bloomfilter2<key_t, hash_fn_0, hash_fn_1, word_t, dtl::mem::numa_allocator<word_t>, 2, false>;
using bf1_k3_t = dtl::bloomfilter2<key_t, hash_fn_0, hash_fn_1, word_t, dtl::mem::numa_allocator<word_t>, 3, false>;
using bf1_k4_t = dtl::bloomfilter2<key_t, hash_fn_0, hash_fn_1, word_t, dtl::mem::numa_allocator<word_t>, 4, false>;
using bf1_k5_t = dtl::bloomfilter2<key_t, hash_fn_0, hash_fn_1, word_t, dtl::mem::numa_allocator<word_t>, 5, false>;
using bf1_k6_t = dtl::bloomfilter2<key_t, hash_fn_0, hash_fn_1, word_t, dtl::mem::numa_allocator<word_t>, 6, false>;
#endif

using bf2_k2_t = dtl::bloomfilter2<key_t, hash_fn_0, hash_fn_1, word_t, dtl::mem::numa_allocator<word_t>, 2, false>;
using bf2_k3_t = dtl::bloomfilter2<key_t, hash_fn_0, hash_fn_1, word_t, dtl::mem::numa_allocator<word_t>, 3, false>;
using bf2_k4_t = dtl::bloomfilter2<key_t, hash_fn_0, hash_fn_1, word_t, dtl::mem::numa_allocator<word_t>, 4, false>;
using bf2_k5_t = dtl::bloomfilter2<key_t, hash_fn_0, hash_fn_1, word_t, dtl::mem::numa_allocator<word_t>, 5, false>;
using bf2_k6_t = dtl::bloomfilter2<key_t, hash_fn_0, hash_fn_1, word_t, dtl::mem::numa_allocator<word_t>, 6, false>;
using bf2_k7_t = dtl::bloomfilter2<key_t, hash_fn_0, hash_fn_1, word_t, dtl::mem::numa_allocator<word_t>, 7, false>;

} // namespace dtl