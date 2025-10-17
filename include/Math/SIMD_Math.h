#pragma once

#include <cstddef>

namespace nre
{
class SIMDMath
{
public:
    static void multiply4x4(const float* lhs, const float* rhs, float* out, std::size_t count);
};
} // namespace nre
