#pragma once

#include <dtl/dtl.hpp>
#include <dtl/simd.hpp>

#include <dtl/filter/blocked_bloomfilter/blocked_bloomfilter_logic.hpp>
#include <dtl/filter/blocked_bloomfilter/hash_family.hpp>

namespace dtl {

GENERATE_EXTERN($u32, 4, 2,  2)
GENERATE_EXTERN($u32, 4, 2,  4)
GENERATE_EXTERN($u32, 4, 2,  6)
GENERATE_EXTERN($u32, 4, 2,  8)
GENERATE_EXTERN($u32, 4, 2, 10)
GENERATE_EXTERN($u32, 4, 2, 12)
GENERATE_EXTERN($u32, 4, 2, 14)
GENERATE_EXTERN($u32, 4, 2, 16)

} // namespace dtl