#include "Math/SIMD_Math.h"

#include <cstring>

namespace nre
{
void SIMDMath::multiply4x4(const float* lhs, const float* rhs, float* out, std::size_t count)
{
    // Placeholder implementation copies lhs into out until SIMD math is implemented.
    std::memcpy(out, lhs, sizeof(float) * 16 * count);
    (void)rhs;
}
} // namespace nre
