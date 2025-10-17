#include "Math/Vector3.h"

#include <cassert>
#include <cmath>

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

Vector3 Vector3::operator+(const Vector3& rhs) const noexcept
{
    return Vector3{x + rhs.x, y + rhs.y, z + rhs.z};
}

Vector3 Vector3::operator-(const Vector3& rhs) const noexcept
{
    return Vector3{x - rhs.x, y - rhs.y, z - rhs.z};
}

Vector3 Vector3::operator*(float scalar) const noexcept
{
    return Vector3{x * scalar, y * scalar, z * scalar};
}

Vector3 Vector3::operator/(float scalar) const noexcept
{
    const float inv = 1.0F / scalar;
    return Vector3{x * inv, y * inv, z * inv};
}

Vector3& Vector3::operator+=(const Vector3& rhs) noexcept
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

Vector3& Vector3::operator-=(const Vector3& rhs) noexcept
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

Vector3& Vector3::operator*=(float scalar) noexcept
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Vector3& Vector3::operator/=(float scalar) noexcept
{
    const float inv = 1.0F / scalar;
    x *= inv;
    y *= inv;
    z *= inv;
    return *this;
}

float Vector3::length() const noexcept
{
    return std::sqrt(lengthSquared());
}

float Vector3::lengthSquared() const noexcept
{
    return x * x + y * y + z * z;
}

Vector3 Vector3::normalized() const noexcept
{
    const float len = length();
    if (len > 0.0F)
    {
        return *this / len;
    }
    return Vector3{};
}

float Vector3::dot(const Vector3& lhs, const Vector3& rhs) noexcept
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

Vector3 Vector3::cross(const Vector3& lhs, const Vector3& rhs) noexcept
{
    return Vector3{lhs.y * rhs.z - lhs.z * rhs.y,
                   lhs.z * rhs.x - lhs.x * rhs.z,
                   lhs.x * rhs.y - lhs.y * rhs.x};
}
} // namespace nre
