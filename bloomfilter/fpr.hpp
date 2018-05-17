#pragma once

#include "math.hpp"

namespace dtl {
namespace bloomfilter {

$f64
fpr(u64 m,
    u64 n,
    u64 k,
    u64 B,    /* block size in bits */
    $u64 S = 0,   /* sector size in bits */
    u64 z = 1,    /* the number of zones */
    u1 self_collisions = true) {

  if (B == 0) {
    // Standard Bloom filter
    return fpr(m, n, k);
  }

  if (S == 0) {
    S = B; // default sector size to block size
  }

  if (S == B && z == 1) {
    // Blocked Bloom filter
    return fpr_blocked(m, n, k, B, self_collisions);
  }

  if (S < B && z == 1) {
    // Sectorized Blocked Bloom filter
    return fpr_blocked_sectorized(m, n, k, B, S, self_collisions);
  }

  if (S < B && z > 1) {
    // Zoned Sectorized Blocked Bloom filter
    return fpr_zoned(m, n, k, B, S, z, self_collisions);
  }

  throw std::invalid_argument("Invalid blocked Bloom filter configuration: B=" + std::to_string(B)
                              + ", S=" + std::to_string(S)
                              + ", z=" + std::to_string(z));
}

} // namespace bloomfilter
} // namespace dtl