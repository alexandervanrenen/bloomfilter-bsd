#pragma once

#include <dtl/dtl.hpp>

namespace dtl {

//===----------------------------------------------------------------------===//
// Pimpl wrapper to reduce compilation time.
//===----------------------------------------------------------------------===//
class bbf_64 {
  class impl;
  std::unique_ptr<impl> pimpl;

 public:

  using key_t = $u32;
  using word_t = $u64;

  //===----------------------------------------------------------------------===//
  // The API functions.
  //===----------------------------------------------------------------------===//
  void
  insert(word_t* __restrict filter_data, key_t key);

  void
  batch_insert(word_t* __restrict filter_data, const key_t* keys, u32 key_cnt);

  $u1
  contains(const word_t* __restrict filter_data, key_t key) const;

  $u64
  batch_contains(const word_t* __restrict filter_data,
                 const key_t* keys, u32 key_cnt,
                 $u32* match_positions, u32 match_offset) const;

  std::string
  name() const;

  std::size_t
  size_in_bytes() const;

  std::size_t
  size() const;

  static void
  calibrate();

  static void
  force_unroll_factor(u32 u);
  //===----------------------------------------------------------------------===//


  bbf_64(std::size_t m, u32 k, u32 word_cnt_per_block = 1, u32 sector_cnt = 1);
  ~bbf_64();
  bbf_64(bbf_64&&);
  bbf_64(const bbf_64&) = delete;
  bbf_64& operator=(bbf_64&&);
  bbf_64& operator=(const bbf_64&) = delete;

};

} // namespace dtl
