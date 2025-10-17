#pragma once

#include <array>

#include "Math/Vector3.h"

namespace nre
{
struct Matrix4
{
    std::array<float, 16> data{1.0F, 0.0F, 0.0F, 0.0F,
                               0.0F, 1.0F, 0.0F, 0.0F,
                               0.0F, 0.0F, 1.0F, 0.0F,
                               0.0F, 0.0F, 0.0F, 1.0F};

    static Matrix4 identity();
    static Matrix4 perspective(float fovRadians, float aspectRatio, float nearPlane, float farPlane);
    static Matrix4 translation(const Vector3& translation);
    static Matrix4 scale(const Vector3& scale);
    static Matrix4 lookAt(const Vector3& eye, const Vector3& target, const Vector3& up);

    Matrix4 operator*(const Matrix4& rhs) const noexcept;
    Matrix4& operator*=(const Matrix4& rhs) noexcept;

    float* dataPtr() noexcept { return data.data(); }
    const float* dataPtr() const noexcept { return data.data(); }

    float& at(int row, int column) noexcept;
    float at(int row, int column) const noexcept;
};
} // namespace nre
