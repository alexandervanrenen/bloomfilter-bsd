#pragma once

#include <cstdlib>

#include <dtl/dtl.hpp>
#include <dtl/hash.hpp>
#include <dtl/math.hpp>

#include <dtl/bloomfilter/bloomfilter_addressing_logic.hpp>
#include <dtl/bloomfilter/cuckoo_filter_helper.hpp>

namespace dtl {


struct cuckoo_filter_word_table {

  using word_t = uint64_t;

  static constexpr uint32_t tag_size_bits = 6;
  static constexpr uint32_t tags_per_bucket = 2;

  static constexpr uint32_t tag_mask = (1u << tag_size_bits) - 1;
  static constexpr uint32_t bucket_size_bits = tag_size_bits * tags_per_bucket;
  static constexpr uint32_t bucket_mask = (1u << bucket_size_bits) - 1;
  static constexpr uint32_t bucket_count = sizeof(word_t) * 8 / bucket_size_bits;
  static constexpr uint32_t bucket_addressing_bits = dtl::ct::log_2_u32<dtl::next_power_of_two(bucket_count)>::value;

  static constexpr uint32_t capacity = bucket_count * tags_per_bucket;


  static constexpr uint32_t null_tag = 0;
  static constexpr uint32_t overflow_tag = uint32_t(-1);
  static constexpr uint32_t overflow_bucket = bucket_mask;

  //===----------------------------------------------------------------------===//
  // Members
  //===----------------------------------------------------------------------===//
  alignas(sizeof(word_t))
  word_t filter;
  //===----------------------------------------------------------------------===//


  /// C'tor
  cuckoo_filter_word_table() : filter(0) { }


  __forceinline__
  uint32_t
  read_bucket(const uint32_t bucket_idx) const {
    auto bucket = filter >> (bucket_size_bits * bucket_idx);
    return static_cast<uint32_t>(bucket);
  }

  __forceinline__
  void
  overflow(const uint32_t bucket_idx) {
    auto existing_bucket = read_bucket(bucket_idx);
    auto b = word_t(overflow_bucket ^ existing_bucket) << (bucket_size_bits * bucket_idx);
    filter ^= b;
  }


  __forceinline__
  uint32_t
  read_tag_from_bucket(const uint32_t bucket, const uint32_t tag_idx) const {
    auto tag = (bucket >> (tag_size_bits * tag_idx)) & tag_mask;
    return static_cast<uint32_t>(tag);
  }


  __forceinline__
  uint32_t
  read_tag(const uint32_t bucket_idx, const uint32_t tag_idx) const {
    auto bucket = read_bucket(bucket_idx);
    auto tag = (bucket >> (tag_size_bits * tag_idx)) & tag_mask;
    return static_cast<uint32_t>(tag);
  }


  __forceinline__
  uint32_t
  write_tag(const uint32_t bucket_idx, const uint32_t tag_idx, const uint32_t tag) {
    auto existing_tag = read_tag(bucket_idx, tag_idx);
    auto t = (word_t(tag ^ existing_tag) << (bucket_size_bits * bucket_idx)) << (tag_size_bits * tag_idx);
    filter ^= t;
    return existing_tag;
  }


  __forceinline__
  uint32_t
  insert_tag_kick_out(const uint32_t bucket_idx, const uint32_t tag) {
    // Check whether this is an overflow bucket.
    auto bucket = read_bucket(bucket_idx);
    if (bucket == overflow_bucket) {
      return overflow_tag;
    }
    // Check the buckets' entries for free space.
    for (uint32_t tag_idx = 0; tag_idx < tags_per_bucket; tag_idx++) {
      auto t = read_tag_from_bucket(bucket, tag_idx);
      if (t == tag) {
        return null_tag;
      }
      else if (t == 0) {
        write_tag(bucket_idx, tag_idx, tag);
        return null_tag;
      }
    }
    // couldn't find an empty place
    // kick out existing tag
    uint32_t rnd_tag_idx = static_cast<uint32_t>(std::rand()) % tags_per_bucket;
    return write_tag(bucket_idx, rnd_tag_idx, tag);
  }


  __forceinline__
  uint32_t
  insert_tag(const uint32_t bucket_idx, const uint32_t tag) {
    // Check whether this is an overflow bucket.
    auto bucket = read_bucket(bucket_idx);
    if (bucket == overflow_bucket) {
      return overflow_tag;
    }
    // Check the buckets' entries for free space.
    for (uint32_t tag_idx = 0; tag_idx < tags_per_bucket; tag_idx++) {
      auto t = read_tag_from_bucket(bucket, tag_idx);
      if (t == tag) {
        return null_tag;
      }
      else if (t == 0) {
        write_tag(bucket_idx, tag_idx, tag);
        return null_tag;
      }
    }
    return tag;
  }


  __forceinline__
  bool
  find_tag_in_buckets(const uint32_t bucket_idx, const uint32_t alternative_bucket_idx, const uint32_t tag) const {
    word_t f = filter;
    const word_t mask = (word_t(bucket_mask) << (bucket_idx * bucket_size_bits))
                      | (word_t(bucket_mask) << (alternative_bucket_idx * bucket_size_bits));
    const auto masked_buckets = f & mask;
//    return packed_value<uint64_t, tag_size_bits>::contains(masked_buckets, tag);
    return packed_value<uint64_t, tag_size_bits>::contains(masked_buckets, tag)
         | packed_value<uint64_t, tag_size_bits>::contains(masked_buckets, bucket_mask); // overflow check
  }

};


} // namespace dtl