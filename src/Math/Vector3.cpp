#include "Math/Vector3.h"

#include <cassert>

namespace nre
{
float& Vector3::operator[](std::size_t index) noexcept
{
    switch (index)
    {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    default:
        assert(false && "Vector3 index out of range");
        return x;
    }
}

float Vector3::operator[](std::size_t index) const noexcept
{
    switch (index)
    {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    default:
        assert(false && "Vector3 index out of range");
        return x;
    }
}
} // namespace nre
