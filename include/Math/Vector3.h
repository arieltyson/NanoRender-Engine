#pragma once

#include <array>
#include <cstddef>

namespace nre
{
struct Vector3
{
    float x = 0.0F;
    float y = 0.0F;
    float z = 0.0F;

    constexpr Vector3() = default;
    constexpr Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    float& operator[](std::size_t index) noexcept;
    float operator[](std::size_t index) const noexcept;

    Vector3 operator+(const Vector3& rhs) const noexcept;
    Vector3 operator-(const Vector3& rhs) const noexcept;
    Vector3 operator*(float scalar) const noexcept;
    Vector3 operator/(float scalar) const noexcept;

    Vector3& operator+=(const Vector3& rhs) noexcept;
    Vector3& operator-=(const Vector3& rhs) noexcept;
    Vector3& operator*=(float scalar) noexcept;
    Vector3& operator/=(float scalar) noexcept;

    float length() const noexcept;
    float lengthSquared() const noexcept;
    Vector3 normalized() const noexcept;

    static float dot(const Vector3& lhs, const Vector3& rhs) noexcept;
    static Vector3 cross(const Vector3& lhs, const Vector3& rhs) noexcept;
};
} // namespace nre
