#include "Math/SIMD_Math.h"

#include <cstddef>

namespace
{
constexpr int kMatrixDimension = 4;

inline float elementAt(const float* matrix, int row, int column)
{
    return matrix[column * kMatrixDimension + row];
}

inline void setElement(float* matrix, int row, int column, float value)
{
    matrix[column * kMatrixDimension + row] = value;
}
} // namespace

namespace nre
{
void SIMDMath::multiply4x4(const float* lhs, const float* rhs, float* out, std::size_t count)
{
    for (std::size_t index = 0; index < count; ++index)
    {
        const float* a = lhs + index * kMatrixDimension * kMatrixDimension;
        const float* b = rhs + index * kMatrixDimension * kMatrixDimension;
        float* result = out + index * kMatrixDimension * kMatrixDimension;

        for (int row = 0; row < kMatrixDimension; ++row)
        {
            for (int column = 0; column < kMatrixDimension; ++column)
            {
                float value = 0.0F;
                for (int inner = 0; inner < kMatrixDimension; ++inner)
                {
                    value += elementAt(a, row, inner) * elementAt(b, inner, column);
                }
                setElement(result, row, column, value);
            }
        }
    }
}
} // namespace nre
