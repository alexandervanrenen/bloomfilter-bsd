#pragma once

#include <cmath>
#include <stdexcept>

#include <dtl/dtl.hpp>
#include <dtl/bloomfilter.hpp>
#include <dtl/bloomfilter_vec.hpp>
#include <dtl/bloomfilter2_vec.hpp>
#include <dtl/hash.hpp>
#include <dtl/mem.hpp>


namespace dtl {

/// A runtime wrapper for a Bloom filter instance.
/// The actual Bloom filter type is determined by the parameters 'm' and 'k'.
/// Note: This should only be used if the parameter is NOT known at compile time.
struct bloomfilter_runtime {

  using key_t = $u32;
  using word_t = $u32;

  /// The bit length of the Bloom filter.
  $u32 m;
  /// The number of hash functions.
  $u32 h;
  /// The number of bits set per entry.
  $u32 k;
  /// Pointer to the Bloom filter instance.
  void* instance;
  /// Pointer to the Bloom filter vector extension.
  void* instance_vec;

  // ---- The API functions. ----
  std::function<void(const key_t /*key*/)>
  insert;

  std::function<$u1(const key_t /*key*/)>
  contains;

  std::function<$u64(const key_t* /*keys*/, u32 /*key_cnt*/, $u32* /*match_positions*/, u32 /*match_offset*/)>
  batch_contains;

  std::function<f64()>
  load_factor;

  std::function<u64()>
  pop_count;

  std::function<void()>
  print_info;

  std::function<void()>
  print;
  // ---- ----

  bloomfilter_runtime() = default;

  bloomfilter_runtime(bloomfilter_runtime&& src)
      : m(src.m), h(src.h), k(src.k),
        instance(src.instance),
        instance_vec(src.instance_vec),
        insert(std::move(src.insert)),
        contains(std::move(src.contains)),
        batch_contains(std::move(src.batch_contains)),
        load_factor(std::move(src.load_factor)),
        pop_count(std::move(src.pop_count)),
        print_info(std::move(src.print_info)),
        print(std::move(src.print)) {
    // invalidate pointers
    src.instance = nullptr;
    src.instance_vec = nullptr;
  }

  bloomfilter_runtime&
  operator=(dtl::bloomfilter_runtime&& src) {
    m = src.m;
    h = src.h;
    k = src.k;
    instance = src.instance;
    instance_vec = src.instance_vec;
    insert = std::move(src.insert);
    contains = std::move(src.contains);
    batch_contains = std::move(src.batch_contains);
    load_factor = std::move(src.load_factor);
    pop_count = std::move(src.pop_count);
    print_info = std::move(src.print_info);
    print = std::move(src.print);
    // invalidate pointers
    src.instance = nullptr;
    src.instance_vec = nullptr;
    return *this;
  }

  template<typename T>
  using hash_fn_0 = dtl::hash::knuth<T>;
//  using hash_fn_0 = dtl::hash::murmur_32<T>;
//  using hash_fn_0 = dtl::hash::identity<T>;

  template<typename T>
  using hash_fn_1 = dtl::hash::knuth_alt<T>;
//  using hash_fn_1 = dtl::hash::knuth<T>;
//  using hash_fn_0 = dtl::hash::identity<T>;


  // The supported Bloom filter implementations. (Note: Sectorization is not supported via the runtime API.)
  using bf1_k1_t = dtl::bloomfilter<key_t, hash_fn_0, word_t, dtl::mem::numa_allocator<word_t>, 1, false>;
  using bf1_k2_t = dtl::bloomfilter<key_t, hash_fn_0, word_t, dtl::mem::numa_allocator<word_t>, 2, false>;
  using bf1_k3_t = dtl::bloomfilter<key_t, hash_fn_0, word_t, dtl::mem::numa_allocator<word_t>, 3, false>;
  using bf1_k4_t = dtl::bloomfilter<key_t, hash_fn_0, word_t, dtl::mem::numa_allocator<word_t>, 4, false>;
  using bf1_k5_t = dtl::bloomfilter<key_t, hash_fn_0, word_t, dtl::mem::numa_allocator<word_t>, 5, false>;
  using bf1_k6_t = dtl::bloomfilter<key_t, hash_fn_0, word_t, dtl::mem::numa_allocator<word_t>, 6, false>;

  using bf2_k2_t = dtl::bloomfilter2<key_t, hash_fn_0, hash_fn_1, word_t, dtl::mem::numa_allocator<word_t>, 2, false>;
  using bf2_k3_t = dtl::bloomfilter2<key_t, hash_fn_0, hash_fn_1, word_t, dtl::mem::numa_allocator<word_t>, 3, false>;
  using bf2_k4_t = dtl::bloomfilter2<key_t, hash_fn_0, hash_fn_1, word_t, dtl::mem::numa_allocator<word_t>, 4, false>;
  using bf2_k5_t = dtl::bloomfilter2<key_t, hash_fn_0, hash_fn_1, word_t, dtl::mem::numa_allocator<word_t>, 5, false>;
  using bf2_k6_t = dtl::bloomfilter2<key_t, hash_fn_0, hash_fn_1, word_t, dtl::mem::numa_allocator<word_t>, 6, false>;
  using bf2_k7_t = dtl::bloomfilter2<key_t, hash_fn_0, hash_fn_1, word_t, dtl::mem::numa_allocator<word_t>, 7, false>;


  // Vectorization related compile time constants.
  static constexpr u32 unroll_factors_avx2_bf1[7]   { 0u,       8u, 4u, 4u, 2u, 2u, 2u };
  static constexpr u32 unroll_factors_avx2_bf2[8]   { 0u, 0u,   2u, 2u, 2u, 1u, 1u, 1u };
  static constexpr u32 unroll_factors_avx512_bf1[7] { 0u,       4u, 4u, 4u, 4u, 4u, 4u };
  static constexpr u32 unroll_factors_avx512_bf2[8] { 0u, 0u,   4u, 4u, 4u, 2u, 2u, 1u };

  // Pick the optimal (empirically determined) unrolling factors for the current architecture.
  static constexpr auto unroll_factors_bf1 = dtl::simd::bitwidth::value == 512 ? unroll_factors_avx512_bf1 : unroll_factors_avx2_bf1;
  static constexpr auto unroll_factors_bf2 = dtl::simd::bitwidth::value == 512 ? unroll_factors_avx512_bf2 : unroll_factors_avx2_bf2;


  // The supported Bloom filter vectorization extensions.
  using bf1_k1_vt = dtl::bloomfilter_vec<key_t, hash_fn_0, word_t, dtl::mem::numa_allocator<word_t>, 1, false, unroll_factors_bf1[1]>;
  using bf1_k2_vt = dtl::bloomfilter_vec<key_t, hash_fn_0, word_t, dtl::mem::numa_allocator<word_t>, 2, false, unroll_factors_bf1[2]>;
  using bf1_k3_vt = dtl::bloomfilter_vec<key_t, hash_fn_0, word_t, dtl::mem::numa_allocator<word_t>, 3, false, unroll_factors_bf1[3]>;
  using bf1_k4_vt = dtl::bloomfilter_vec<key_t, hash_fn_0, word_t, dtl::mem::numa_allocator<word_t>, 4, false, unroll_factors_bf1[4]>;
  using bf1_k5_vt = dtl::bloomfilter_vec<key_t, hash_fn_0, word_t, dtl::mem::numa_allocator<word_t>, 5, false, unroll_factors_bf1[5]>;
  using bf1_k6_vt = dtl::bloomfilter_vec<key_t, hash_fn_0, word_t, dtl::mem::numa_allocator<word_t>, 6, false, unroll_factors_bf1[6]>;

  using bf2_k2_vt = dtl::bloomfilter2_vec<key_t, hash_fn_0, hash_fn_1, word_t, dtl::mem::numa_allocator<word_t>, 2, false, unroll_factors_bf2[2]>;
  using bf2_k3_vt = dtl::bloomfilter2_vec<key_t, hash_fn_0, hash_fn_1, word_t, dtl::mem::numa_allocator<word_t>, 3, false, unroll_factors_bf2[3]>;
  using bf2_k4_vt = dtl::bloomfilter2_vec<key_t, hash_fn_0, hash_fn_1, word_t, dtl::mem::numa_allocator<word_t>, 4, false, unroll_factors_bf2[4]>;
  using bf2_k5_vt = dtl::bloomfilter2_vec<key_t, hash_fn_0, hash_fn_1, word_t, dtl::mem::numa_allocator<word_t>, 5, false, unroll_factors_bf2[5]>;
  using bf2_k6_vt = dtl::bloomfilter2_vec<key_t, hash_fn_0, hash_fn_1, word_t, dtl::mem::numa_allocator<word_t>, 6, false, unroll_factors_bf2[6]>;
  using bf2_k7_vt = dtl::bloomfilter2_vec<key_t, hash_fn_0, hash_fn_1, word_t, dtl::mem::numa_allocator<word_t>, 7, false, unroll_factors_bf2[7]>;


  template<
      typename bf_t, // the scalar bloomfilter type
      typename bf_vt // the vector extension for the bloomfilter
  >
  void
  _construct_and_bind(u64 m) {
    using namespace std::placeholders;

    // Instantiate a Bloom filter.
    bf_t* bf = new bf_t(m);
    instance = bf;

    // Instantiate the vectorized extension.
    bf_vt* bf_v = new bf_vt { *bf };
    instance_vec = bf_v;

    // Bind the API functions.
    insert = std::bind(&bf_t::insert, bf, _1);
    contains = std::bind(&bf_t::contains, bf, _1);
    //                                                        | picks the default vector length as configured above
    batch_contains = std::bind(&bf_vt::template batch_contains<>, bf_v, _1, _2, _3, _4);
    load_factor = std::bind(&bf_t::load_factor, bf);
    pop_count = std::bind(&bf_t::popcnt, bf);
    print_info = std::bind(&bf_t::print_info, bf);
    print = std::bind(&bf_t::print, bf);
  }


  template<
      typename bf_t, // the scalar bloomfilter type
      typename bf_vt // the vector extension for the bloomfilter
  >
  void
  _copy_and_bind(bloomfilter_runtime& copy,
                 const dtl::mem::allocator_config allocator_config) const {
    using namespace std::placeholders;

    // Copy the Bloom filter.
    const bf_t* bf_src = static_cast<bf_t*>(instance);
    auto allocator = dtl::mem::numa_allocator<word_t>(allocator_config);
    bf_t* bf_dst = bf_src->make_heap_copy(allocator);
    copy.instance = bf_dst;

    // Instantiate the vectorized extension.
    bf_vt* bf_v = new bf_vt { *bf_dst };
    copy.instance_vec = bf_v;

    // Bind the API functions.
    copy.insert = std::bind(&bf_t::insert, bf_dst, _1);
    copy.contains = std::bind(&bf_t::contains, bf_dst, _1);
    copy.batch_contains = std::bind(&bf_vt::template batch_contains<>, bf_v, _1, _2, _3, _4);
    copy.load_factor = std::bind(&bf_t::load_factor, bf_dst);
    copy.pop_count = std::bind(&bf_t::popcnt, bf_dst);
    copy.print_info = std::bind(&bf_t::print_info, bf_dst);
    copy.print = std::bind(&bf_t::print, bf_dst);
  }


  template<
      typename bf_t, // the scalar bloomfilter type
      typename bf_vt // the vector extension for the bloomfilter
  >
  void
  _destruct() {
    bf_vt* bf_v = static_cast<bf_vt*>(instance_vec);
    delete bf_v;
    instance_vec = nullptr;
    bf_t* bf = static_cast<bf_t*>(instance);
    delete bf;
    instance = nullptr;
  }


  /// Instantiate a Bloom filter based on the given parameters 'k' and 'm'.
  static
  bloomfilter_runtime
  construct(u32 k, u64 m) {

    // Determine the number of bits required to identify the individual words of the Bloom filter.
    using bf_t = bf1_k1_t;
    u64 actual_m = bf_t::determine_actual_length(m);
    u64 word_bit_cnt = dtl::log_2(actual_m / bf_t::word_bitlength);

    // Determine the number of hash functions needed.
    // Note: Currently only 1 or 2 hash functions are supported.
    u64 hash_fn_cnt = ((bf_t::hash_value_bitlength - word_bit_cnt) / (bf_t::bit_cnt_per_k * k)) > 0 ? 1 : 2;

    bloomfilter_runtime wrapper;
    wrapper.m = actual_m;
    wrapper.h = hash_fn_cnt;
    wrapper.k = k;
    switch (wrapper.h) {
      case 1:
        // Instantiate a Bloom filter with one hash function.
        switch (wrapper.k) {
          case 1: wrapper._construct_and_bind<bf1_k1_t, bf1_k1_vt>(m); break;
          case 2: wrapper._construct_and_bind<bf1_k2_t, bf1_k2_vt>(m); break;
          case 3: wrapper._construct_and_bind<bf1_k3_t, bf1_k3_vt>(m); break;
          case 4: wrapper._construct_and_bind<bf1_k4_t, bf1_k4_vt>(m); break;
          case 5: wrapper._construct_and_bind<bf1_k5_t, bf1_k5_vt>(m); break;
          case 6: wrapper._construct_and_bind<bf1_k6_t, bf1_k6_vt>(m); break;
          default:
            throw std::invalid_argument("The given 'k' is not supported.");
        }
        break;
      case 2:
        // Instantiate a Bloom filter with two hash functions.
        switch (wrapper.k) {
          // k must be > 1, otherwise bf1 should be used.
          case 2: wrapper._construct_and_bind<bf2_k2_t, bf2_k2_vt>(m); break;
          case 3: wrapper._construct_and_bind<bf2_k3_t, bf2_k3_vt>(m); break;
          case 4: wrapper._construct_and_bind<bf2_k4_t, bf2_k4_vt>(m); break;
          case 5: wrapper._construct_and_bind<bf2_k5_t, bf2_k5_vt>(m); break;
          case 6: wrapper._construct_and_bind<bf2_k6_t, bf2_k6_vt>(m); break;
          case 7: wrapper._construct_and_bind<bf2_k7_t, bf2_k7_vt>(m); break;
          default:
            throw std::invalid_argument("The given 'k' is not supported.");
        }
        break;
      default:
        unreachable();
    }
    return wrapper;
  }


  /// Create a copy of the Bloom filter (allows to specify a different memory allocation strategy).
  bloomfilter_runtime
  make_copy(const dtl::mem::allocator_config allocator_config) {
    bloomfilter_runtime copy;
    copy.h = h;
    copy.k = k;
    copy.m = m;
    switch (h) {
      case 1:
        switch (k) {
          case 1: _copy_and_bind<bf1_k1_t, bf1_k1_vt>(copy, allocator_config); break;
          case 2: _copy_and_bind<bf1_k2_t, bf1_k2_vt>(copy, allocator_config); break;
          case 3: _copy_and_bind<bf1_k3_t, bf1_k3_vt>(copy, allocator_config); break;
          case 4: _copy_and_bind<bf1_k4_t, bf1_k4_vt>(copy, allocator_config); break;
          case 5: _copy_and_bind<bf1_k5_t, bf1_k5_vt>(copy, allocator_config); break;
          case 6: _copy_and_bind<bf1_k6_t, bf1_k6_vt>(copy, allocator_config); break;
          default:
            throw std::invalid_argument("The given 'k' is not supported.");
        }
        break;
      case 2:
        switch (k) {
          case 2: _copy_and_bind<bf2_k2_t, bf2_k2_vt>(copy, allocator_config); break;
          case 3: _copy_and_bind<bf2_k3_t, bf2_k3_vt>(copy, allocator_config); break;
          case 4: _copy_and_bind<bf2_k4_t, bf2_k4_vt>(copy, allocator_config); break;
          case 5: _copy_and_bind<bf2_k5_t, bf2_k5_vt>(copy, allocator_config); break;
          case 6: _copy_and_bind<bf2_k6_t, bf2_k6_vt>(copy, allocator_config); break;
          case 7: _copy_and_bind<bf2_k7_t, bf2_k7_vt>(copy, allocator_config); break;
          default:
            throw std::invalid_argument("The given 'k' is not supported.");
        }
        break;
      default:
        throw std::invalid_argument("The given 'h' is not supported.");
    }
    return copy;
  }


  /// Destruct the Bloom filter instance.
  void
  destruct() {
    if (!is_initialized()) return;
    switch (h) {
      case 1:
        switch (k) {
          case 1: _destruct<bf1_k1_t, bf1_k1_vt>(); break;
          case 2: _destruct<bf1_k2_t, bf1_k2_vt>(); break;
          case 3: _destruct<bf1_k3_t, bf1_k3_vt>(); break;
          case 4: _destruct<bf1_k4_t, bf1_k4_vt>(); break;
          case 5: _destruct<bf1_k5_t, bf1_k5_vt>(); break;
          case 6: _destruct<bf1_k6_t, bf1_k6_vt>(); break;
          default:
            throw std::invalid_argument("The given 'k' is not supported.");
        }
        break;
      case 2:
        switch (k) {
          case 2: _destruct<bf2_k2_t, bf2_k2_vt>(); break;
          case 3: _destruct<bf2_k3_t, bf2_k3_vt>(); break;
          case 4: _destruct<bf2_k4_t, bf2_k4_vt>(); break;
          case 5: _destruct<bf2_k5_t, bf2_k5_vt>(); break;
          case 6: _destruct<bf2_k6_t, bf2_k6_vt>(); break;
          case 7: _destruct<bf2_k7_t, bf2_k7_vt>(); break;
          default:
            throw std::invalid_argument("The given 'k' is not supported.");
        }
        break;
      default:
        throw std::invalid_argument("The given 'h' is not supported.");
    }

  }


  /// Returns 'true' if the Bloom filter is initialized, 'false' otherwise.
  forceinline
  u1
  is_initialized() {
    return instance != nullptr;
  }


  /// Computes an approximation of the false positive probability.
  /// Assuming independence for the probabilities of each bit being set,
  /// which is not the case in the current implementation.
  f64
  false_positive_probability(u64 element_cnt) {
    auto n = element_cnt;
    return std::pow(1.0 - std::pow(1.0 - (1.0 / m), k * n), k);
  }

};

} // namespace dtl
