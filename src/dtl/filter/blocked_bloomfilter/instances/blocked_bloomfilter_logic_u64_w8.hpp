#pragma once

#include <dtl/dtl.hpp>
#include <dtl/simd.hpp>

#include <dtl/filter/blocked_bloomfilter/blocked_bloomfilter_logic.hpp>
#include <dtl/filter/blocked_bloomfilter/hash_family.hpp>

namespace dtl {

GENERATE_EXTERN($u64, 8, 1,  1)
GENERATE_EXTERN($u64, 8, 1,  2)
GENERATE_EXTERN($u64, 8, 1,  3)
GENERATE_EXTERN($u64, 8, 1,  4)
GENERATE_EXTERN($u64, 8, 1,  5)
GENERATE_EXTERN($u64, 8, 1,  6)
GENERATE_EXTERN($u64, 8, 1,  7)
GENERATE_EXTERN($u64, 8, 1,  8)
GENERATE_EXTERN($u64, 8, 1,  9)
GENERATE_EXTERN($u64, 8, 1, 10)
GENERATE_EXTERN($u64, 8, 1, 11)
GENERATE_EXTERN($u64, 8, 1, 12)
GENERATE_EXTERN($u64, 8, 1, 13)
GENERATE_EXTERN($u64, 8, 1, 14)
GENERATE_EXTERN($u64, 8, 1, 15)
GENERATE_EXTERN($u64, 8, 1, 16)
GENERATE_EXTERN($u64, 8, 2,  2)
GENERATE_EXTERN($u64, 8, 2,  4)
GENERATE_EXTERN($u64, 8, 2,  6)
GENERATE_EXTERN($u64, 8, 2,  8)
GENERATE_EXTERN($u64, 8, 2, 10)
GENERATE_EXTERN($u64, 8, 2, 12)
GENERATE_EXTERN($u64, 8, 2, 14)
GENERATE_EXTERN($u64, 8, 2, 16)
GENERATE_EXTERN($u64, 8, 4,  4)
GENERATE_EXTERN($u64, 8, 4,  8)
GENERATE_EXTERN($u64, 8, 4, 12)
GENERATE_EXTERN($u64, 8, 4, 16)
GENERATE_EXTERN($u64, 8, 8,  8)
GENERATE_EXTERN($u64, 8, 8, 16)
GENERATE_EXTERN($u64, 8,16, 16)

} // namespace dtl